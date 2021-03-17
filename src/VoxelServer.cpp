#include "VoxelServer.h"

VoxelServer* VoxelServer::m_instance = NULL;
bool VoxelServer::m_Gpu_Acceloration = false;
std::string VoxelServer::m_app_dir = "";
std::string VoxelServer::m_cmdArgs = "";
bool VoxelServer::m_running = false;

VoxelServer::VoxelServer(char * args)
{
   m_instance = this;

   c_context = NULL;
   com_executer = NULL;
   
   m_cmdArgs = args;
   frameCounter = 0;
   curCommand = "";
   executedCommand = "";

#ifdef WINDOWS_BUILD
   char pBuf[256];
   size_t len = sizeof(pBuf);
   int bytes = GetModuleFileName(NULL, pBuf, len);

#else // linux
   int bytes = MIN(readlink("/proc/self/exe", pBuf, len), len - 1);
   if (bytes >= 0)
      pBuf[bytes] = '\0';
   
#endif

   m_app_dir = pBuf;
   m_app_dir = m_app_dir.substr(0, m_app_dir.find_last_of('\\'));
   //printf(pBuf);
}

void VoxelServer::SetCurrentCommand(std::string command)
{
    curCommand = command;
    //Logger::Log("current: " + curCommand);
}

void VoxelServer::Start()
{
   Init();
   Loop();
}

void VoxelServer::LoadSettings(std::string file)
{
}

void VoxelServer::Init()
{
   m_running = true;

   com_executer = new CommandExecuter();
   com_executer->Run(false);

   Logger::Init();
   Logger::Log("Server Started!");
   


   ShaderFileManager::Init(AppDirectory() + sepChar() + "shaders" + sepChar());
   ShaderFileManager::Load("TerrainModule.sl");
   Logger::Log("Loaded terrain module file.");

   SmoothVoxelSettings::Init();

   c_context = ComputeEngine::GetNewContext();
   Logger::Log("OpenCL context created.");

   c_context->Add_Program("TerrainModule", ShaderFileManager::Get("TerrainModule.s").c_str());
   Logger::Log("TerrainModule program created.");

   c_context->Programs("TerrainModule")->AddConstant("X_NOISE_GEN", "1619");
   c_context->Programs("TerrainModule")->AddConstant("Y_NOISE_GEN", "31337");
   c_context->Programs("TerrainModule")->AddConstant("Z_NOISE_GEN", "6971");
   c_context->Programs("TerrainModule")->AddConstant("SEED_NOISE_GEN", "1013");
   c_context->Programs("TerrainModule")->AddConstant("SHIFT_NOISE_GEN", "8");

   c_context->Programs("TerrainModule")->AddConstant("PI", "3.1415926535897932385");
   c_context->Programs("TerrainModule")->AddConstant("SQRT_2", "1.4142135623730950488");
   c_context->Programs("TerrainModule")->AddConstant("SQRT_3", "1.7320508075688772935");
   c_context->Programs("TerrainModule")->AddConstant("DEG_TO_RAD", "0.01745329");
   c_context->Programs("TerrainModule")->AddConstant("RAD_TO_DEG", "57.29578");
   c_context->Programs("TerrainModule")->AddConstant("QUALITY_FAST", "0");
   c_context->Programs("TerrainModule")->AddConstant("QUALITY_STD", "1");
   c_context->Programs("TerrainModule")->AddConstant("QUALITY_BEST", "2");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_FREQUENCY", "1.0");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_LACUNARITY", "2.0");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_OCTAVE_COUNT", "6");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_PERSISTENCE", "0.5");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_QUALITY", "QUALITY_STD");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_SEED", "0");
   c_context->Programs("TerrainModule")->AddConstant("MAX_OCTAVE", "30");
   c_context->Programs("TerrainModule")->AddConstant("MAX_CONTROL_POINTS", "10");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_TURBULENCE_FREQUENCY", "DEFAULT_FREQUENCY");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_TURBULENCE_POWER", "1.0");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_TURBULENCE_ROUGHNESS", "3");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_TURBULENCE_SEED", "DEFAULT_SEED");
   c_context->Programs("TerrainModule")->AddConstant("DEFAULT_VORONOI_DISPLACEMENT", "1.0");
   Logger::Log("Set Terrain Module global settings.");

   Logger::Log("Server Initialized Successfully!");
}

void VoxelServer::Loop()
{
    timer.restart();
    lastTime = timer.elapsed();
   while (m_running)
   {
      double curTime = timer.elapsed();
      double dt = curTime - lastTime;
      lastTime = curTime;

      Sleep(1);
      Update(dt);
      frameCounter++;
   }
}

void VoxelServer::Update(double dt)
{
   if (!m_running)
      return;

   com_executer->Process();

   //Logger::Log("Elapsed: " + std::to_string(dt) + ", Total: " + std::to_string(timer.elapsed()));

   if (executedCommand != "")
   {
       

       // TODO: Add commands.

       executedCommand = "";
   }

   Logger::Update();
}

void VoxelServer::Stop()
{
   m_running = false;

   Dispose();
}

void VoxelServer::Dispose()
{
   c_context->Dispose();
}
