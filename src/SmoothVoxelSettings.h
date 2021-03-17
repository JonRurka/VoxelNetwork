#pragma once

class SmoothVoxelSettings
{
public:
   static int seed;
   static int radius;
   static bool randomSeed;
   static bool enableCaves;
   static bool enableGrass;
   static float caveDensity;
   static float amplitude;
   static float groundOffset;
   static float grassOffset;
   
   static double voxelsPerMeter ;
   static int MeterSizeX;
   static int MeterSizeY;
   static int MeterSizeZ;
   static int ChunkSizeX;
   static int ChunkSizeY;
   static int ChunkSizeZ;
   static float half;//(float)((1 / voxelsPerMeter) / 2);
   
   static int maxChunksX;
   static int maxChunksY;
   static int maxChunksZ;
   
   static int ChunksPerRegionX;
   static int ChunksPerRegionZ;

   static void Init(int p_seed = 0)
   {
      seed = p_seed;
      radius = 30;
      randomSeed = false;
      enableCaves = true;
      enableGrass = false;
      caveDensity = 14;
      amplitude = 250;
      groundOffset = 280;
      grassOffset = 4;

      voxelsPerMeter = 1.0;
      MeterSizeX = 16;
      MeterSizeY = 256;
      MeterSizeZ = 16;
      ChunkSizeX = (int)(MeterSizeX * voxelsPerMeter);
      ChunkSizeY = (int)(MeterSizeY * voxelsPerMeter);
      ChunkSizeZ = (int)(MeterSizeZ * voxelsPerMeter);
      half = 0;//(float)((1 / voxelsPerMeter) / 2);

      maxChunksX = 10;
      maxChunksY = 1;
      maxChunksZ = 10;

      ChunksPerRegionX = 10;
      ChunksPerRegionZ = 10;
   }
};