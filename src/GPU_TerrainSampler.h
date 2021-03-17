#pragma once

#include "ISampler.h"


#include "Compute/ComputeEngine.h"

class GPU_TerrainSampler : public ISampler
{
public:
   struct Result
   {
      float iso;
      uint type;

      static int Size()
      {
         return sizeof(float) + sizeof(unsigned int);
      }
   };

   struct Settings {
      int ChunkSizeX;
      int ChunkSizeY;
      int ChunkSizeZ;

      float VoxelsPerMeter;
      int quality;
      int seed;
      bool enableCaves;
      float amp;
      float caveDensity;
      float grassOffset;

      float noiseX;
      float noiseZ;

      int Y_Min;
      int Y_Max;
      int xStart;
      int zStart;
      int y_height;
   };

private:

   int y_height;

   Result* iso_types;

   ComputeContext* c_context;

   ComputeKernel* CS_Heightmap;
   ComputeKernel* CS_ISO_Type_Map;

   


public:


   int seed;
   bool enableCaves;
   float amp;
   float caveDensity;
   float grassOffset;

   float* SurfaceData;
   bool SurfaceSet = false;

   double VoxelsPerMeter;
   int ChunkSizeX;
   int ChunkSizeY;
   int ChunkSizeZ;

   float min;
   float max;

   int ymin;
   int ymax;

   bool SurfaceExtracted;
   bool ISO_Extracted;
   bool Extract;

   ComputeBuffer* height_buffer;
   ComputeBuffer* iso_buffer;
   ComputeBuffer* type_buffer;
   ComputeBuffer* iso_type_buffer;

   GPU_TerrainSampler() {

   }
   GPU_TerrainSampler(int _seed, bool _enableCaves, float _amp, float _caveDensity, float _grassOffset);
   

   void SetChunkSettings(double voxelsPerMeter, Vector3Int chunkSizes, Vector3Int chunkMeterSize, int skipDist, float half, Vector3 sideLength);
   double Noise(module::Module* module, float x, float y, float z, double scale, double height, double power);
   double GetHeight(int x, int y);
   float* SetSurfaceData(Vector2Int bottomLeft, Vector2Int topRight);
   float* SetSurfaceData(float* data);
   float* GetSurfaceData();
   double GetSurfaceHeight(int LocalX, int LocalZ);
   double GetMin();
   double GetMax();
   double GetIsoValue(Vector3Int LocalPosition, Vector3Int globalLocation, unsigned int &type);

   void ComputeNoiseGrid(int Y_Min, int Y_Max, int xStart, int zStart);

   void Dispose();
};

