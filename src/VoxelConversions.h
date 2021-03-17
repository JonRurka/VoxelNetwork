#pragma once

#include "stdafx.h"
#include "VectorsAndStuff.h"
#include "SmoothVoxelSettings.h"

class VoxelConversions {
public:
   static Vector3 ChunkCoordToWorld(Vector3Int location) {
      //return new Vector3(location.x * VoxelSettings.ChunkSizeX / VoxelSettings.voxelsPerMeter - VoxelSettings.half, location.y * VoxelSettings.ChunkSizeY / VoxelSettings.voxelsPerMeter - VoxelSettings.half, location.z * VoxelSettings.ChunkSizeZ / VoxelSettings.voxelsPerMeter - VoxelSettings.half);
      return Vector3(location.x * SmoothVoxelSettings::MeterSizeX - SmoothVoxelSettings::half, location.y * SmoothVoxelSettings::MeterSizeY - SmoothVoxelSettings::half, location.z * SmoothVoxelSettings::MeterSizeZ - SmoothVoxelSettings::half);
   }

   static Vector3Int WorldPosToChunkCoord(Vector3 location) {
      return Vector3Int(round(location.x / SmoothVoxelSettings::MeterSizeX + SmoothVoxelSettings::half), round(location.y / SmoothVoxelSettings::MeterSizeY + SmoothVoxelSettings::half), round(location.y / SmoothVoxelSettings::MeterSizeZ + SmoothVoxelSettings::half));
   }

   static Vector3Int GlobalToLocalChunkCoord(Vector3Int location) {
      Vector3Int ChunkCoord = VoxelToChunk(location);
      return GlobalToLocalChunkCoord(ChunkCoord, location);
   }

   static Vector3Int GlobalToLocalChunkCoord(Vector3Int ChunkCoord, Vector3Int location) {
      int x = location.x - (ChunkCoord.x * SmoothVoxelSettings::ChunkSizeX);
      int y = location.y - (ChunkCoord.y * SmoothVoxelSettings::ChunkSizeY);
      int z = location.z - (ChunkCoord.z * SmoothVoxelSettings::ChunkSizeZ);
      return Vector3Int(x, y, z);
   }

   static Vector3Int LocalToGlobalCoord(Vector3Int Chunk, Vector3Int location) {
      int x = location.x + (Chunk.x * SmoothVoxelSettings::ChunkSizeX);
      int y = location.y + (Chunk.y * SmoothVoxelSettings::ChunkSizeY);
      int z = location.z + (Chunk.z * SmoothVoxelSettings::ChunkSizeZ);
      return Vector3Int(x, y, z);
   }

   static Vector2Int ChunkToRegion(Vector3Int location)
   {
      int x = floor(location.x / (float)SmoothVoxelSettings::ChunksPerRegionX);
      int z = floor(location.z / (float)SmoothVoxelSettings::ChunksPerRegionZ);
      return Vector2Int(x, z);
   }

   static Vector3Int RegionToChunk(Vector2Int location)
   {
      int x = ((location.x) * SmoothVoxelSettings::ChunksPerRegionX);
      int z = ((location.y) * SmoothVoxelSettings::ChunksPerRegionZ);
      return Vector3Int(x, 0, z);
   }

   static Vector3Int VoxelToChunk(Vector3Int location) {
      int x = floor(location.x / (float)SmoothVoxelSettings::ChunkSizeX);
      int y = floor(location.y / (float)SmoothVoxelSettings::ChunkSizeY);
      int z = floor(location.z / (float)SmoothVoxelSettings::ChunkSizeZ);
      return Vector3Int(x, y, z);
   }

   static Vector3Int ChunkToVoxel(Vector3Int location)
   {
      int x = ((location.x) * SmoothVoxelSettings::ChunkSizeX);
      int y = ((location.y) * SmoothVoxelSettings::ChunkSizeY);
      int z = ((location.z) * SmoothVoxelSettings::ChunkSizeZ);
      return Vector3Int(x, y, z);
   }

   static Vector3 VoxelToWorld(Vector3Int location)
   {
      return VoxelToWorld(location.x, location.y, location.z);
   }

   static Vector3 VoxelToWorld(int x, int y, int z) {
      float newX = (((x / (float)SmoothVoxelSettings::voxelsPerMeter) - SmoothVoxelSettings::half));
      float newY = (((y / (float)SmoothVoxelSettings::voxelsPerMeter) - SmoothVoxelSettings::half));
      float newZ = (((z / (float)SmoothVoxelSettings::voxelsPerMeter) - SmoothVoxelSettings::half));
      return Vector3(newX, newY, newZ);
   }

   static Vector3Int WorldToVoxel(Vector3 worldPos)
   {
      int x = floor(((worldPos.x) + SmoothVoxelSettings::half) * (float)SmoothVoxelSettings::voxelsPerMeter);
      int y = floor(((worldPos.y) + SmoothVoxelSettings::half) * (float)SmoothVoxelSettings::voxelsPerMeter);
      int z = floor(((worldPos.z) + SmoothVoxelSettings::half) * (float)SmoothVoxelSettings::voxelsPerMeter);
      return Vector3Int(x, y, z);
   }

   static float Scale(float value, float oldMin, float oldMax, float newMin, float newMax)
   {
      return newMin + (value - oldMin) * (newMax - newMin) / (oldMax - oldMin);
   }
};
