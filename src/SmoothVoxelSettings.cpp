#include "SmoothVoxelSettings.h"

int SmoothVoxelSettings::seed = 0;
int SmoothVoxelSettings::radius;
bool SmoothVoxelSettings::randomSeed;
bool SmoothVoxelSettings::enableCaves;
bool SmoothVoxelSettings::enableGrass;
float SmoothVoxelSettings::caveDensity;
float SmoothVoxelSettings::amplitude;
float SmoothVoxelSettings::groundOffset;
float SmoothVoxelSettings::grassOffset;

double SmoothVoxelSettings::voxelsPerMeter;
int SmoothVoxelSettings::MeterSizeX;
int SmoothVoxelSettings::MeterSizeY;
int SmoothVoxelSettings::MeterSizeZ;
int SmoothVoxelSettings::ChunkSizeX;
int SmoothVoxelSettings::ChunkSizeY;
int SmoothVoxelSettings::ChunkSizeZ;
float SmoothVoxelSettings::half;//(float)((1 / voxelsPerMeter) / 2);

int SmoothVoxelSettings::maxChunksX;
int SmoothVoxelSettings::maxChunksY;
int SmoothVoxelSettings::maxChunksZ;

int SmoothVoxelSettings::ChunksPerRegionX;
int SmoothVoxelSettings::ChunksPerRegionZ;