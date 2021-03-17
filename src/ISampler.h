#pragma once

#include "libnoise/noise.h"
#include "VectorsAndStuff.h"

using namespace noise;

class ISampler
{
public:
   virtual void SetChunkSettings(double voxelsPerMeter, Vector3Int chunkSizes, Vector3Int chunkMeterSize, int skipDist, float half, Vector3 sideLength) = 0;
   virtual double Noise(module::Module* module, float x, float y, float z, double scale, double height, double power) = 0;
   virtual double GetHeight(int x, int y) = 0;
   virtual float* SetSurfaceData(Vector2Int bottomLeft, Vector2Int topRight) = 0;
   virtual float* SetSurfaceData(float* data) = 0;
   virtual float* GetSurfaceData() = 0;
   virtual double GetSurfaceHeight(int LocalX, int LocalZ) = 0;
   virtual double GetMin() = 0;
   virtual double GetMax() = 0;
   virtual double GetIsoValue(Vector3Int LocalPosition, Vector3Int globalLocation, unsigned int &type) = 0;
};

