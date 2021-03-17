#include "GPU_ColumnBuilder.h"

GPU_ColumnBuilder::GPU_ColumnBuilder() 
{
   Result = new ColumnResult();
}

GPU_ColumnBuilder::GPU_ColumnBuilder(ColumnResult* result) {
   Result = result;
}

GPU_ColumnBuilder::GPU_ColumnBuilder(ISampler* sampler) {
   Result = new ColumnResult();
   Sampler = (GPU_TerrainSampler*)sampler;
}

GPU_ColumnBuilder::GPU_ColumnBuilder(ColumnResult* result, ISampler* sampler) {
   Result = result;
   Sampler = (GPU_TerrainSampler*)sampler;
}

void GPU_ColumnBuilder::Init(Vector3Int location, float voxelsPerMeter, int chunkMeterSizeX, int chunkMeterSizeY, int chunkMeterSizeZ)
{
   Location = location;

   VoxelsPerMeter = voxelsPerMeter;
   ChunkMeterSizeX = chunkMeterSizeX;
   ChunkMeterSizeY = chunkMeterSizeY;
   ChunkMeterSizeZ = chunkMeterSizeZ;

   ChunkSizeX = (int)(ChunkMeterSizeX * VoxelsPerMeter);
   ChunkSizeY = (int)(ChunkMeterSizeY * VoxelsPerMeter);
   ChunkSizeZ = (int)(ChunkMeterSizeZ * VoxelsPerMeter);
   //Debug.LogFormat("GPU Init: {0} x {1} x {2}, {3}", ChunkMeterSizeX, ChunkMeterSizeY, ChunkMeterSizeZ, VoxelsPerMeter);

   half = ((1.0f / (float)VoxelsPerMeter) / 2.0f);
   xSideLength = ChunkMeterSizeX / (float)ChunkSizeX;
   ySideLength = ChunkMeterSizeY / (float)ChunkSizeY;
   zSideLength = ChunkMeterSizeZ / (float)ChunkSizeZ;
   skipDist = round(1 / (float)VoxelsPerMeter);

   Sampler->Extract = false;

   // TODO: some shader stuff

}

float* GPU_ColumnBuilder::GenerateHeightMap()
{
   if (!SurfaceGenerated)
   {
      Vector2Int bottomLeft = Vector2Int(Location.x * ChunkSizeX, Location.z * ChunkSizeZ);
      Vector2Int topRight = Vector2Int(Location.x * ChunkSizeX + ChunkSizeX, Location.z * ChunkSizeZ + ChunkSizeZ);
      Result->SurfaceData = Sampler->SetSurfaceData(bottomLeft, topRight);
      SurfaceGenerated = true;

      Result->Min = (int)Sampler->GetMin();
      Result->Max = (int)Sampler->GetMax();

      return Result->SurfaceData;
   }
   return Result->SurfaceData;
}

ColumnResult* GPU_ColumnBuilder::Generate(int Y_Min, int Y_Max, int xStart, int zStart)
{
   Sampler->ComputeNoiseGrid(Y_Min, Y_Max, xStart, zStart);

   return NULL;
}

void GPU_ColumnBuilder::Dispose() {
   deactivated = true;
   if (Sampler != NULL)
      Sampler->Dispose();
   Result->Dispose();
}
