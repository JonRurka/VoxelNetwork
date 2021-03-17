#include "ComputeEngine.h"

cl_platform_id ComputeEngine::platform_id = 0;
cl_uint ComputeEngine::num_of_platforms = 0;
cl_context_properties ComputeEngine::properties[3] = {0};
cl_device_id ComputeEngine::device_ids[MAX_DEVICES] = { 0 };
cl_uint ComputeEngine::num_of_devices = 0;
std::string  ComputeEngine::app_dir;

int ComputeEngine::Init(int maxDevices, std::string dir)
{
   num_of_platforms = 0;
   num_of_devices = 0;

   // retreives a list of platforms available
   if (clGetPlatformIDs(1, &platform_id, &num_of_platforms) != CL_SUCCESS)
   {
      return 1;
   }

   // context properties list - must be terminated with 0
   properties[0] = CL_CONTEXT_PLATFORM;
   properties[1] = (cl_context_properties)platform_id;
   properties[2] = 0;


   // try to get a supported GPU device
   //ComputeEngine::device_ids = new cl_device_id[num_of_devices];
   if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, maxDevices, device_ids, &num_of_devices) != CL_SUCCESS)
   {
      return 2;
   }

   

   app_dir = dir;

   return 0;
}

ComputeContext* ComputeEngine::GetNewContext() {
   return new ComputeContext(properties, device_ids);
}

ComputeContext::ComputeContext(cl_context_properties properties[3], cl_device_id* device_ids)
{
   cl_int err;
   numContexts = 0;

   //context = new cl_context[ComputeEngine::GetNumDevices()];
   //command_queue = new cl_command_queue[ComputeEngine::GetNumDevices()];

   context = clCreateContext(properties, ComputeEngine::GetNumDevices(), device_ids, NULL, NULL, &err);

   for (int i = 0; i < ComputeEngine::GetNumDevices(); i++)
   {
      command_queue[i] = clCreateCommandQueue(context, device_ids[i], 0, &err);
      numContexts++;
   }


}

/*int ComputeContext::SetProgram(const char* source)
{
   cl_int err;

   //program = new cl_program[numContexts];

   // create a program from the kernel source code
   for (int i = 0; i < numContexts; i++)
   {
      program[i] = clCreateProgramWithSource(context, 1, (const char **)&source, NULL, &err);
      // compile the program
      if (clBuildProgram(program[i], 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
      {
         //printf("Error building program\n");
         return 1;
      }
   }

   return 0;
   
}*/

ComputeProgram* ComputeContext::Add_Program(std::string name, const char* source)
{
   ComputeProgram* program = new ComputeProgram(context, command_queue, numContexts);
   int res = program->Set(source);
   programs[name] = program;
   return program;
}

ComputeProgram* ComputeContext::Programs(std::string name)
{
   return programs[name];
}

ComputeKernel* ComputeContext::GetKernel(std::string p_name, std::string name)
{
   return Programs(p_name)->GetKernel(name);
}

ComputeProgram::ComputeProgram(cl_context context, cl_command_queue queue[MAX_DEVICES], int numDevs)
{
   numDevices = numDevs;
   m_context = context;

   for (int i = 0; i < numDevices; i++)
   {
      command_queue[i] = queue[i];
   }
}

int ComputeProgram::Set(const char* source)
{
   cl_int err;
   program = clCreateProgramWithSource(m_context, 1, (const char **)&source, NULL, &err);
   return err;
}

void ComputeProgram::AddConstant(std::string name, std::string value)
{
   args += "-D" + name + "=" + value + " ";
}

int ComputeProgram::Build(char* errorStr, size_t e_size)
{
   std::string inc_dir = ComputeEngine::GetAppDir() + "\\shaders\\include\\";
   args += "-I " + inc_dir;
   int build_res = clBuildProgram(program, 0, NULL, args.c_str(), NULL, NULL);

   size_t ret_e_size;
   int res = clGetProgramBuildInfo(program, *ComputeEngine::GetDevice(0), CL_PROGRAM_BUILD_LOG, e_size, errorStr, &ret_e_size);

   return build_res;
}

ComputeKernel* ComputeProgram::GetKernel(std::string k_name)
{
   if (kernels.count(k_name) > 0)
   {
      return kernels[k_name];
   }

   ComputeKernel* new_kern = new ComputeKernel((char*)k_name.c_str(), command_queue, program, numDevices);
   kernels[k_name] = new_kern;
   return new_kern;
}

ComputeBuffer* ComputeContext::GetBuffer(ComputeBuffer::Buffer_Type type, size_t size)
{
   cl_mem_flags flags;

   switch (type)
   {
   case ComputeBuffer::READ:
      flags = CL_MEM_READ_ONLY;
      break;

   case ComputeBuffer::Write:
      flags = CL_MEM_WRITE_ONLY;
      break;

   case ComputeBuffer::Read_Write:
      flags = CL_MEM_READ_WRITE;
      break;

   default:
      return NULL;
   }

   return new ComputeBuffer(context, command_queue, numContexts, flags, size);
}

ComputeKernel::ComputeKernel(char* name, cl_command_queue queue[MAX_DEVICES], cl_program program, int numPrograms)
{
   cl_int err;
   numKernels = numPrograms;
   m_program = program;

   //kernels = new cl_kernel[numKernels];
   //command_queue = new cl_command_queue[numKernels];

   for (int i = 0; i < numPrograms; i++)
   {
      command_queue[i] = queue[i];
   }
   
   kernel = clCreateKernel(m_program, name, &err);

}

int ComputeKernel::SetBuffer(int device, ComputeBuffer* buffer, int arg)
{
   int res = clSetKernelArg(kernel, arg, sizeof(cl_mem), buffer->Get_CL_Mem());
   return res;
}

int ComputeKernel::Execute(int device, cl_uint work_dim, size_t* global_work_size)
{
   cl_command_queue c_q = command_queue[device];
   int res = clEnqueueNDRangeKernel(c_q, kernel, work_dim, NULL, global_work_size, NULL, 0, NULL, NULL);
   res = clFinish(command_queue[device]);
   return res;
}

ComputeBuffer::ComputeBuffer(cl_context contexts, cl_command_queue queues[MAX_DEVICES], int numContext, cl_mem_flags type, size_t length)
{
   num = numContext;
   //buffer = new cl_mem[numContext];
   //command_queue = new cl_command_queue[numContext];
   size = length;

   context = contexts;

   cl_int err;
   for (int i = 0; i < numContext; i++)
   {
      command_queue[i] = queues[i];
      
   }

   buffer = clCreateBuffer(context, type, length, NULL, &err);
}

void ComputeBuffer::SetData(void* data)
{
   for (int i = 0; i < num; i++)
   {
      SetData(i, data);
   }
}

int ComputeBuffer::SetData(int device, void* data)
{
   int res = clEnqueueWriteBuffer(command_queue[device], buffer, CL_TRUE, 0, size, data, 0, NULL, NULL);
   return res;
}

int ComputeBuffer::GetData(int device, void* outData)
{
   int res = clEnqueueReadBuffer(command_queue[device], buffer, CL_TRUE, 0, size, outData, 0, NULL, NULL);
   return res;
}