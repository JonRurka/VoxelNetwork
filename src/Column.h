#pragma once

#include "stdafx.h"
#include "enums.h"
#include "VectorsAndStuff.h"
#include "ISampler.h"
#include "GPU_ColumnBuilder.h"
#include "VoxelServer.h"
#include "VoxelConversions.h"



class Column
{
private:
   unsigned int m_World_ID;
   Vector3Int m_Location;
   std::string m_ColumnFile;
   ISampler* m_Sampler;
   bool m_FullyLoaded;
   //Region Region;
   LOD_Mode m_Current_Mode;
   LOD_Mode m_Max_Mode;
   int m_GeneratedBlocks;
   bool m_ReduceDepth;
   float m_Depth;
   bool m_LoadedFromDisk;

   bool m_Initialized;
   bool m_SurfaceGenerated;
   bool m_deactivated;

   IColumnBuilder* m_builder;
   ColumnResult* m_col_data;

   float m_VoxelsPerMeter;
   int m_ChunkMeterSizeX;
   int m_ChunkMeterSizeY;
   int m_ChunkMeterSizeZ;
   int m_ChunkSizeX;
   int m_ChunkSizeY;
   int m_ChunkSizeZ;
   int m_skipDist;
   float m_half;
   float m_xSideLength;
   float m_ySideLength;
   float m_zSideLength;
   bool m_empty;

   const Vector3Int directionOffsets[8] = {
      Vector3Int(0, 0, 1),
      Vector3Int(1, 0, 1),
      Vector3Int(1, 0, 0),
      Vector3Int(0, 0, 0),
      Vector3Int(0, 1, 1),
      Vector3Int(1, 1, 1),
      Vector3Int(1, 1, 0),
      Vector3Int(0, 1, 0),
   };

   const int edgeTable[256] =
   { 0x0 , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
   0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
   0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
   0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
   0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
   0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
   0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
   0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
   0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
   0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
   0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
   0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
   0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
   0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
   0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
   0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
   0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
   0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
   0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
   0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
   0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
   0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
   0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
   0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
   0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
   0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
   0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
   0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
   0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
   0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
   0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
   0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0 };

   Vector3 m_locOffset[8];
   Vector3Int m_globalOffsets[8];


   UpdateMode m_updateMode;
public:

   Vector3Int Get_Location() {
      return m_Location;
   }

   std::string Get_ColumnFile() {
      return m_ColumnFile;
   }

   ISampler* Get_Sampler()
   {
      return m_Sampler;
   }

   int Max() {
      return m_col_data->Max;
   }

   int Min() {
      return m_col_data->Max;
   }

   char* blocks_type() {
      return m_col_data->blocks_type;
   }

   float* blocks_iso() {
      return m_col_data->blocks_iso;
   }

   bool* blocks_set() {
      return m_col_data->blocks_set;
   }

   bool* blocks_surface(){
      return m_col_data->blocks_surface;
   }

   uint* surfaceBlocks() {
      return m_col_data->surfaceBlocks;
   }

   int surfaceBlocksCount() {
      return m_col_data->surfaceBlocksCount;
   }

   float* SurfaceData() {
      return m_col_data->SurfaceData;
   }

   Column(unsigned int world_id, char* folder, Vector3Int location)
   {
      m_World_ID = world_id;
      m_Location = location;
      m_ColumnFile = GetColumnFile(folder, location);
      m_Sampler = VoxelServer::Instance()->GetSampler(m_World_ID);
   }

   void Init(float voxelsPerMeter, int chunkMeterSizeX, int chunkMeterSizeY, int chunkMeterSizeZ);

   void BuildChunk(LOD_Mode mode = LOD_Mode_Full);

   void GenerateHeightMap();

   void Generate();

   void MarkAsSet(int x, int y, int z) {
      if (!m_deactivated)
         blocks_set()[Get_Flat_Index(x, y, z)] = true;
   }

   void SetBlock(int x, int y, int z, Block block) {
      if (!m_deactivated)
      {
         int index = Get_Flat_Index(x, y, z);
         blocks_type()[index] = (char)block.type;
         blocks_iso()[index] = block.iso;
      }
   }

   void SetBlockValue(int x, int y, int z, float value) {
      if (!m_deactivated)
      {
         blocks_iso()[Get_Flat_Index(x, y, z)] = value;
      }
   }

   float GetBlockValue(int x, int y, int z) {
      if (!m_deactivated)
      {
         return blocks_iso()[Get_Flat_Index(x, y, z)];
      }
      return 0;
   }

   bool IsBlockSet(int x, int y, int z) {
      if (!m_deactivated)
         return blocks_set()[Get_Flat_Index(x, y, z)];
      return false;
   }

   bool IsInBounds(int x, int y, int z)
   {
      return ((x < m_ChunkSizeX) && x >= 0) && ((y < m_ChunkSizeY) && y >= 0) && ((z < m_ChunkSizeZ) && z >= 0);
   }

   int Get_Flat_Index(int x, int y, int z)
   {
      return x + m_ChunkSizeX * (y + m_ChunkSizeY * z);
   }

   void Serialize();

   LOD_Mode Deserialize(LOD_Mode load_mode);

private:

   void CalculateVariables();

   static std::string GetColumnFile(std::string folder, Vector3Int location)
   {
      int f_len = folder.length() - 1;
      folder = folder[f_len] == VoxelServer::sepChar() ? folder : folder + std::string(1, VoxelServer::sepChar());
      return folder + "column_" + location.File_String() + ".clm";
   }
};

