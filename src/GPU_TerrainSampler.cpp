#include "GPU_TerrainSampler.h"

GPU_TerrainSampler::GPU_TerrainSampler(int _seed, bool _enableCaves, float _amp, float _caveDensity, float _grassOffset)
{
   Extract = true;

   seed = _seed;
   enableCaves = _enableCaves;
   amp = _amp;
   caveDensity = _caveDensity;
   grassOffset = _grassOffset;

   //min = std::numeric_limits<float>::min();
   //max = std::numeric_limits<float>::max();

   c_context = ComputeEngine::GetNewContext();
   c_context->Add_Program("p", "");

   CS_Heightmap = c_context->GetKernel("p", "CS_Heightmap");
   CS_ISO_Type_Map = c_context->GetKernel("p", "CS_ISO_Type_Map");
}

void GPU_TerrainSampler::SetChunkSettings(double voxelsPerMeter, Vector3Int chunkSizes, Vector3Int chunkMeterSize, int skipDist, float half, Vector3 sideLength)
{
   if (SurfaceSet)
      return;

   VoxelsPerMeter = voxelsPerMeter;
   ChunkSizeX = chunkSizes.x;
   ChunkSizeY = chunkSizes.y;
   ChunkSizeZ = chunkSizes.z;

   SurfaceData = new float[(ChunkSizeX + 2) * (ChunkSizeZ + 2)];

   // set all the settings.
}

double GPU_TerrainSampler::Noise(noise::module::Module* module, float x, float y, float z, double scale, double height, double power)
{
   return 0;
}

double GPU_TerrainSampler::GetHeight(int x, int y)
{
   return 0;
}

float* GPU_TerrainSampler::SetSurfaceData(Vector2Int bottomLeft, Vector2Int topRight)
{
   return 0;
}

float* GPU_TerrainSampler::SetSurfaceData(float* data)
{
   return 0;
}

float* GPU_TerrainSampler::GetSurfaceData()
{
   return 0;
}

double GPU_TerrainSampler::GetSurfaceHeight(int LocalX, int LocalZ)
{
   return 0;
}

double GPU_TerrainSampler::GetMin()
{
   return 0;
}

double GPU_TerrainSampler::GetMax()
{
   return 0;
}

double GPU_TerrainSampler::GetIsoValue(Vector3Int LocalPosition, Vector3Int globalLocation, unsigned int &type)
{
   return 0;
}

void GPU_TerrainSampler::ComputeNoiseGrid(int Y_Min, int Y_Max, int xStart, int zStart) {

}

void GPU_TerrainSampler::Dispose()
{
}

void Dispose() {

}