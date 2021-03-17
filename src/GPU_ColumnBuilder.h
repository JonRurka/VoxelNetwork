#pragma once

#include "ISampler.h"
#include "GPU_TerrainSampler.h"

class ColumnResult
{
public:
   char* blocks_type;

   float* SurfaceData;

   float* blocks_iso;

   bool* blocks_set;

   bool* blocks_surface;

   unsigned int* surfaceBlocks;

   int Min;
   int Max;

   int surfaceBlocksCount;

private:
   bool allocated;

public:
   void Allocate(int ChunkSizeX, int ChunkSizeY, int ChunkSizeZ)
   {
      if (allocated)
         return;
      allocated = true;

      int size = ChunkSizeX * ChunkSizeY * ChunkSizeZ;
      blocks_iso = new float[size];
      blocks_type = new char[size];
      blocks_set = new bool[size];
      blocks_surface = new bool[size];
      surfaceBlocks = new unsigned int[size];
   }

   void Dispose()
   {
      delete blocks_type;
      delete blocks_type;
      delete SurfaceData;
      delete blocks_iso;
      delete blocks_set;
      delete blocks_surface;
      delete surfaceBlocks;
      surfaceBlocksCount = -1;
   }
};

class IColumnBuilder 
{
public:
   virtual float* GenerateHeightMap() = 0;

   virtual ColumnResult* Generate(int Y_Min, int Y_Max, int xStart, int zStart) = 0;
};



class GPU_ColumnBuilder : public IColumnBuilder
{

   float VoxelsPerMeter;
   int ChunkMeterSizeX;
   int ChunkMeterSizeY;
   int ChunkMeterSizeZ;
   int ChunkSizeX;
   int ChunkSizeY;
   int ChunkSizeZ;
   int skipDist;
   float half;
   float xSideLength;
   float ySideLength;
   float zSideLength;
   bool deactivated;

public:

   struct Debug_Data_Res
   {
      Vector3 p1;
      Vector3 p2;
   };

   GPU_TerrainSampler* Sampler;

   ColumnResult* Result;

   Vector3Int Location;

   bool SurfaceGenerated;

   GPU_ColumnBuilder();
   GPU_ColumnBuilder(ColumnResult* result);
   GPU_ColumnBuilder(ISampler* sampler);
   GPU_ColumnBuilder(ColumnResult* result, ISampler* sampler);

   void Init(Vector3Int location, float voxelsPerMeter, int chunkMeterSizeX, int chunkMeterSizeY, int chunkMeterSizeZ);
   float* GenerateHeightMap();
   ColumnResult* Generate(int Y_Min, int Y_Max, int xStart, int zStart);
   void Dispose();
};

