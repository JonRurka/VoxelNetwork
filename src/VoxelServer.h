#pragma once

#include "stdafx.h"
#include "GPU_TerrainSampler.h"
#include "SmoothVoxelSettings.h"
#include "Network/AsyncServer.h"
#include "ShaderFileManager.h"
#include "Logger.h"
#include "CommandExecuter.h"

#include <boost/timer.hpp>

class CommandExecuter;
class Logger;

class VoxelServer
{
private:
   static VoxelServer* m_instance;
   static bool m_Gpu_Acceloration;
   static std::string m_cmdArgs;
   static std::string m_app_dir;
   static bool m_running;

   CommandExecuter* com_executer;

   ComputeContext* c_context;

   std::string curCommand;
   std::string executedCommand;
   int frameCounter;
   boost::timer timer;
   double lastTime;
public:

   static VoxelServer* Instance() {
      return m_instance;
   }

   static void Regions() {

   }

   static bool Gpu_Acceloration() {
      return m_Gpu_Acceloration;
   }

   static CommandExecuter* GetInputProcessor()
   {
       return m_instance->com_executer;
   }

   static bool Gpu_Acceloration(bool enabled) {
      m_Gpu_Acceloration = enabled;
      return m_Gpu_Acceloration;
   }

   static char sepChar() {
#ifdef WINDOWS_BUILD
       return '\\';
#else // linux
       return '/';
#endif
      
   }

   static std::string AppDirectory() {
      return m_app_dir;
   }

   static std::string CmdArgs()
   {
      return "";
   }

   static bool Running()
   {
      return false;
   }

   static double Elapsed() {
       return m_instance->timer.elapsed();
   }

   static ComputeContext* GetComputeContext() {
       return m_instance->c_context;
   }

   void SetCurrentCommand(std::string command);

   void SetCommand(std::string command) 
   {
       executedCommand = command;
   }

   VoxelServer(char* args);

   ISampler* GetSampler(unsigned int world_id);

   void Start();

   void LoadSettings(std::string file);

   void Init();

   void Loop();

   void Update(double dt);

   void Stop();

   void Dispose();
};

