#include "stdafx.h"

using namespace std;


#define DATA_SIZE 10
#define ERROR_SIZE 100000

void Basic_CL_Test()
{
#if false



   const char *KernelSource =
      "__kernel void hello(__global float *input, __global float *output)\n"\
      "{\n"\
      "  size_t id = get_global_id(0);\n"
      "  output[id] = input[id] * input[id];\n"
      "}\n"\
      "\n";

   cl_context context;
   cl_context_properties properties[3];
   cl_kernel kernel;
   cl_command_queue command_queue;
   cl_program program;
   cl_int err;
   cl_uint num_of_platforms = 0;
   cl_platform_id platform_id;
   cl_device_id device_id;
   cl_uint num_of_devices = 0;
   cl_mem input, output;
   size_t global;

   float inputData[DATA_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
   float results[DATA_SIZE] = { 0 };

   int i;

   // retreives a list of platforms available
   if (clGetPlatformIDs(1, &platform_id, &num_of_platforms) != CL_SUCCESS)
   {
      printf("Unable to get platform_id\n");
      return 1;
   }



   // try to get a supported GPU device
   if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 4, &device_id, &num_of_devices) != CL_SUCCESS)
   {
      printf("Unable to get device_id\n");
      return 1;
   }

   // context properties list - must be terminated with 0
   properties[0] = CL_CONTEXT_PLATFORM;
   properties[1] = (cl_context_properties)platform_id;
   properties[2] = 0;

   // create a context with the GPU device

   context = clCreateContext(properties, 1, &device_id, NULL, NULL, &err);

   // create command queue using the context and device
   command_queue = clCreateCommandQueue(context, device_id, 0, &err);

   // create a program from the kernel source code
   program = clCreateProgramWithSource(context, 1, (const char **)&KernelSource, NULL, &err);


   // compile the program
   if (clBuildProgram(program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
   {
      printf("Error building program\n");
      return 1;
   }

   // specify which kernel from the program to execute
   kernel = clCreateKernel(program, "hello", &err);

   // create buffers for the input and ouput
   input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * DATA_SIZE, NULL, NULL);
   output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * DATA_SIZE, NULL, NULL);

   // load data into the input buffer
   clEnqueueWriteBuffer(command_queue, input, CL_TRUE, 0, sizeof(float) * DATA_SIZE, inputData, 0, NULL, NULL);

   // set the argument list for the kernel command
   clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
   clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
   global = DATA_SIZE;

   // enqueue the kernel command for execution
   clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
   clFinish(command_queue);

   // copy the results from out of the output buffer
   clEnqueueReadBuffer(command_queue, output, CL_TRUE, 0, sizeof(float) * DATA_SIZE, results, 0, NULL, NULL);



   // print the results
   printf("output: ");
   for (i = 0; i < DATA_SIZE; i++)
   {
      printf("%f ", results[i]);
   }

   // cleanup - release OpenCL resources
   clReleaseMemObject(input);
   clReleaseMemObject(output);
   clReleaseProgram(program);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(command_queue);
   clReleaseContext(context);
#endif
}

void Library_CL_Test()
{
#if false
   float other_inputData[DATA_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
   float other_results[DATA_SIZE] = { 1 };
   global = DATA_SIZE;

   ComputeEngine::Init(1, server->AppDirectory());

   char platformInfo[1000];

   clGetPlatformInfo(ComputeEngine::GetPlatform(), CL_PLATFORM_VERSION, 1000, platformInfo, NULL);
   cout << "\nOpenCL Version: " << platformInfo << "\n";



   ComputeContext* c_context = ComputeEngine::GetNewContext();

   std::string fileText;
   std::fstream script_file;

   std::string script_dir = server->AppDirectory() + server->sepChar() + "shaders\\TerrainModule.sl";
   script_file.open(script_dir, ios::in);
   if (!script_file)
   {
      cout << "\nShader not found: " + script_dir;
      return 0;
   }

   std::string tmpTxt;
   while (getline(script_file, tmpTxt))
      fileText += tmpTxt + "\n";
   script_file.close();
   //cout << "\nShader file: " + fileText + "\n";



   char error_str[ERROR_SIZE];

   c_context->Add_Program("p", fileText.c_str());
   c_context->Programs("p")->AddConstant("TEST", "4");
   c_context->Programs("p")->AddConstant("X_NOISE_GEN", "1619");
   c_context->Programs("p")->AddConstant("Y_NOISE_GEN", "31337");
   c_context->Programs("p")->AddConstant("Z_NOISE_GEN", "6971");
   c_context->Programs("p")->AddConstant("SEED_NOISE_GEN", "1013");
   c_context->Programs("p")->AddConstant("SHIFT_NOISE_GEN", "8");

   c_context->Programs("p")->AddConstant("PI", "3.1415926535897932385");
   c_context->Programs("p")->AddConstant("SQRT_2", "1.4142135623730950488");
   c_context->Programs("p")->AddConstant("SQRT_3", "1.7320508075688772935");
   c_context->Programs("p")->AddConstant("DEG_TO_RAD", "0.01745329");
   c_context->Programs("p")->AddConstant("RAD_TO_DEG", "57.29578");
   c_context->Programs("p")->AddConstant("QUALITY_FAST", "0");
   c_context->Programs("p")->AddConstant("QUALITY_STD", "1");
   c_context->Programs("p")->AddConstant("QUALITY_BEST", "2");
   c_context->Programs("p")->AddConstant("DEFAULT_FREQUENCY", "1.0");
   c_context->Programs("p")->AddConstant("DEFAULT_LACUNARITY", "2.0");
   c_context->Programs("p")->AddConstant("DEFAULT_OCTAVE_COUNT", "6");
   c_context->Programs("p")->AddConstant("DEFAULT_PERSISTENCE", "0.5");
   c_context->Programs("p")->AddConstant("DEFAULT_QUALITY", "QUALITY_STD");
   c_context->Programs("p")->AddConstant("DEFAULT_SEED", "0");
   c_context->Programs("p")->AddConstant("MAX_OCTAVE", "30");
   c_context->Programs("p")->AddConstant("MAX_CONTROL_POINTS", "10");
   c_context->Programs("p")->AddConstant("DEFAULT_TURBULENCE_FREQUENCY", "DEFAULT_FREQUENCY");
   c_context->Programs("p")->AddConstant("DEFAULT_TURBULENCE_POWER", "1.0");
   c_context->Programs("p")->AddConstant("DEFAULT_TURBULENCE_ROUGHNESS", "3");
   c_context->Programs("p")->AddConstant("DEFAULT_TURBULENCE_SEED", "DEFAULT_SEED");
   c_context->Programs("p")->AddConstant("DEFAULT_VORONOI_DISPLACEMENT", "1.0");
   int res = c_context->Programs("p")->Build(error_str, ERROR_SIZE);

   cout << error_str;

   ComputeKernel* c_kernel = c_context->GetKernel("p", "hello");

   ComputeBuffer* c_input = c_context->GetBuffer(ComputeBuffer::Buffer_Type::READ, sizeof(float) * DATA_SIZE);
   c_input->SetData(0, other_inputData);

   ComputeBuffer* c_output = c_context->GetBuffer(ComputeBuffer::Buffer_Type::Write, sizeof(float) * DATA_SIZE);

   c_kernel->SetBuffer(0, c_input, 0);
   c_kernel->SetBuffer(0, c_output, 1);


   int got = c_kernel->Execute(0, 1, &global);

   int out_res = c_output->GetData(0, other_results);

   printf("\noutput2 (");
   printf("%i", got);
   printf("): ");
   for (i = 0; i < DATA_SIZE; i++)
   {
      printf("%f ", other_results[i]);
   }

   c_context->Dispose();
#endif
}