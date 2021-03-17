#pragma once

#include "../stdafx.h"
#include "CL/cl.h"


#pragma comment(lib, "OpenCL.lib")

//#include "ComputeContext.h"

class ComputeContext;
class ComputeEngine;
class ComputeKernel;
class ComputeBuffer;

#define MAX_DEVICES  10

class ComputeBuffer
{
   cl_context context;
   cl_mem buffer;
   cl_command_queue command_queue[MAX_DEVICES];
   int num;
   size_t size;

public:
   enum Buffer_Type
   {
      READ = 0,
      Write = 1,
      Read_Write = 2
   };
   

   ComputeBuffer(cl_context context, cl_command_queue queus[MAX_DEVICES], int numContext, cl_mem_flags type, size_t length);

   cl_mem* Get_CL_Mem()
   {
      return &buffer;
   }

   void SetData(void* data);
   int SetData(int device, void* data);

   int GetData(int device, void* outData);

   void Dispose()
   {
      clReleaseMemObject(buffer);
   }
};


class ComputeKernel
{
   cl_program m_program;
   cl_kernel kernel;
   cl_command_queue command_queue[MAX_DEVICES];
   int numKernels;

public:
   ComputeKernel(char* name, cl_command_queue command_queue[MAX_DEVICES], cl_program program, int numPrograms);

   int SetBuffer(int device, ComputeBuffer* buffer, int arg);

   int Execute(int device, cl_uint work_dim, size_t* global_work_size);

   void Dispose()
   {
      clReleaseKernel(kernel);
   }
};

class ComputeProgram {
   std::map<std::string, ComputeKernel*> kernels;
   cl_program program;
   cl_context m_context;
   cl_command_queue command_queue[MAX_DEVICES];
   int numDevices;


   std::string args;

public:
   ComputeProgram(cl_context context, cl_command_queue queue[MAX_DEVICES], int numDevs);

   int Set(const char* source);

   void AddConstant(std::string name, std::string value);

   int Build(char* errorStr, size_t e_size);

   ComputeKernel* GetKernel(std::string name);

   void Dispose()
   {
      for (const auto&[key, value] : kernels) {
         value->Dispose();
      }

      clReleaseProgram(program);
   }
};

class ComputeContext {
   std::map<std::string, ComputeProgram*> programs;
   cl_context context;
   cl_command_queue command_queue[MAX_DEVICES];
   int numContexts;

public:
   ComputeContext(cl_context_properties properties[3], cl_device_id* device_ids);

   //int SetProgram(const char* source);
   ComputeProgram* Add_Program(std::string name, const char* source);
   ComputeProgram* Programs(std::string name);

   ComputeKernel* GetKernel(std::string p_name, std::string name);
   ComputeBuffer* GetBuffer(ComputeBuffer::Buffer_Type type, size_t length);

   void Dispose()
   {

      for (const auto&[key, value] : programs) {
         value->Dispose();
      }

      for (int i = 0; i < numContexts; i++)
      {
         clReleaseCommandQueue(command_queue[i]);
      }

      clReleaseContext(context);
      
   }
};


class ComputeEngine
{
   static cl_platform_id platform_id;
   static cl_uint num_of_platforms;
   static cl_context_properties properties[3];
   static cl_device_id device_ids[MAX_DEVICES];
   static cl_uint num_of_devices;

   static std::string app_dir;

public:
   static int Init(int maxDevices, std::string dir);
   static ComputeContext* GetNewContext();

   static cl_platform_id GetPlatform()
   {
      return platform_id;
   }

   static int GetNumDevices() {
      return num_of_devices;
   }

   static std::string GetAppDir()
   {
      return app_dir;
   }

   static cl_device_id* GetDevice(int i)
   {
      return &device_ids[i];
   }
};


