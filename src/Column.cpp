#include "Column.h"

void Column::Init(float voxelsPerMeter, int chunkMeterSizeX, int chunkMeterSizeY, int chunkMeterSizeZ) {
   m_VoxelsPerMeter = voxelsPerMeter;
   m_ChunkMeterSizeX = chunkMeterSizeX;
   m_ChunkMeterSizeY = chunkMeterSizeY;
   m_ChunkMeterSizeZ = chunkMeterSizeZ;
   CalculateVariables();
   m_Sampler->SetChunkSettings(m_VoxelsPerMeter,
      Vector3Int(m_ChunkSizeX, m_ChunkSizeY, m_ChunkSizeZ),
      Vector3Int(m_ChunkMeterSizeX, m_ChunkMeterSizeY, m_ChunkMeterSizeZ),
      m_skipDist,
      m_half,
      Vector3(m_xSideLength, m_ySideLength, m_zSideLength));

   m_col_data = new ColumnResult();
   m_col_data->SurfaceData = new float[(m_ChunkSizeX + 2) * (m_ChunkSizeZ + 2)];

   if (VoxelServer::Instance()->Gpu_Acceloration())
   {
      m_builder = new GPU_ColumnBuilder(m_col_data, m_Sampler);
      ((GPU_ColumnBuilder*)m_builder)->Init(m_Location, m_VoxelsPerMeter, m_ChunkMeterSizeX, m_ChunkMeterSizeY, m_ChunkMeterSizeZ);
   }
   else
   {
      // TODO:

      /*m_builder = new StandardColumnBuilder(col_data, Sampler);
      _builder.Init(Location, VoxelsPerMeter, ChunkMeterSizeX, ChunkMeterSizeY, ChunkMeterSizeZ);
      builder = _builder;*/
   }
}

void Column::BuildChunk(LOD_Mode mode)
{
   //Logger.Log("BuildChunk - mode: {0}, Current_Mode: {1}, Max_Mode:{2}", mode, Current_Mode, Max_Mode);
   if (mode <= m_Current_Mode)
      return;

   if (mode > m_Current_Mode && mode <= m_Max_Mode)
   {
      Deserialize(mode);
      return;
   }

   // mode must be greater than max
   if (m_Current_Mode == LOD_Mode_Empty)
   {
      switch (mode)
      {
      case LOD_Mode_Heightmap:
         m_updateMode = UpdateMode_EmptyToHeightmap;
         break;

      case LOD_Mode_ReducedDepth:
         m_updateMode = UpdateMode_EmptyToReduced;
         break;

      case LOD_Mode_Full:
         m_updateMode = UpdateMode_HeightmapToFull;
         break;
      }
   }
   else if (m_Current_Mode == LOD_Mode_Heightmap)
   {
      switch (mode)
      {
      case LOD_Mode_ReducedDepth:
         m_updateMode = UpdateMode_HeightmapToReduced;
         break;

      case LOD_Mode_Full:
         m_updateMode = UpdateMode_HeightmapToFull;
         break;
      }
   }
   else if (m_Current_Mode == LOD_Mode_ReducedDepth)
   {
      m_updateMode = UpdateMode_ReducedToFull;
   }
   else
      return;


   m_Max_Mode = mode;
   m_Current_Mode = mode;




   GenerateHeightMap();

   if (m_Max_Mode == LOD_Mode_Heightmap)
      return;
   if (m_Max_Mode == LOD_Mode_ReducedDepth)
      m_ReduceDepth = true;

   Generate();

   if (m_Max_Mode == LOD_Mode_Full)
   {
      m_FullyLoaded = true;
   }
}

void Column::GenerateHeightMap() {
   if (!m_SurfaceGenerated)
   {
      //UnityGameServer.Logger.Log("Heightmap Generting...");
      m_builder->GenerateHeightMap();
      m_SurfaceGenerated = true;
   }
}

void Column::Generate()
{
   if (m_empty)
      return;

   if (!m_Initialized)
      return;

   //Stopwatch watch = new Stopwatch();
   //watch.Start();

   //surfaceBlocksCount = 0;
   m_GeneratedBlocks = 0;

   int cx = m_Location.x;
   int cy = m_Location.y;
   int cz = m_Location.z;

   int xStart = cx * m_ChunkSizeX;
   int xEnd = cx * m_ChunkSizeX + m_ChunkSizeX;

   int yStart = cy * m_ChunkSizeY;
   int yEnd = cy * m_ChunkSizeY + m_ChunkSizeY;

   int zStart = cz * m_ChunkSizeZ;
   int zEnd = cz * m_ChunkSizeZ + m_ChunkSizeZ;

   int m_globalLocX = 0;
   int m_globalLocY = 0;
   int m_globalLocZ = 0;

   int x = 0;
   int y = 0;
   int z = 0;

   int Y_Min = 0;
   int Y_Max = m_ChunkSizeY;

   float sampler_y = (float)m_Sampler->GetMin() - m_Depth;
   float heightmapMin_Y = std::max<float>(0, sampler_y);
   int heightmapMin = VoxelConversions::WorldToVoxel(Vector3(0, heightmapMin_Y, 0)).y;
   //Logger.Log("heightmapMin: {0}", heightmapMin);

   int heightmapMin_local = VoxelConversions::GlobalToLocalChunkCoord(Vector3Int(0, heightmapMin, 0)).y;
   //Logger.Log("heightmapMin_local: {0}", heightmapMin_local);

   float meter_size_y = SmoothVoxelSettings::MeterSizeY;
   sampler_y = (float)m_Sampler->GetMax();
   float heightmapMax_y = std::min<float>(meter_size_y, sampler_y);
   int heightmapMax = VoxelConversions::WorldToVoxel(Vector3(0, heightmapMax_y, 0)).y;
   //Logger.Log("heightmapMax: {0}", heightmapMax);

   int heightmapMax_local = VoxelConversions::GlobalToLocalChunkCoord(Vector3Int(0, heightmapMax, 0)).y;
   //Logger.Log("heightmapMax_local: {0}", heightmapMax_local);

   //Logger.Log("Generating with updated mode {0}: {1}", updateMode.ToString(), DebugTimer.Elapsed());

   if (m_updateMode == UpdateMode_EmptyToHeightmap)
   {
      return;
   }
   else if (m_updateMode == UpdateMode_EmptyToReduced || m_updateMode == UpdateMode_EmptyToFull ||
      m_updateMode == UpdateMode_HeightmapToReduced || m_updateMode == UpdateMode_HeightmapToFull)
   {
      //Min = int.MaxValue;
      //Max = int.MinValue;
      switch (m_updateMode)
      {
      case UpdateMode_EmptyToReduced:
      case UpdateMode_HeightmapToReduced:
         Y_Min = heightmapMin_local;
         yStart += Y_Min;
         Y_Max = heightmapMax_local;
         break;

      case UpdateMode_EmptyToFull:
      case UpdateMode_HeightmapToFull:
         Y_Max = heightmapMax_local;
         break;
      }
   }
   else if (m_updateMode == UpdateMode_ReducedToFull)
   {
      Y_Max = heightmapMin_local;
      //Logger.Log("ReducedToFull: " + heightmapMin_local);
   }

   //else if(updateMode == UpdateMode.)

   //if (LoadedFromDisk && !ReduceDepth && Max_Mode == LOD_Mode.Full)
   //    Y_Max = VoxelConversions.GlobalToLocalChunkCoord(VoxelConversions.WorldToVoxel(new Vector3(0, (float)Sampler.GetMin() - Depth, 0))).y;

   //UnityGameServer.Logger.Log("Column: Y_Min: {0}, Y_Max: {1}", Y_Min, Y_Max);

   m_col_data->Allocate(m_ChunkSizeX, m_ChunkSizeY, m_ChunkSizeZ);
   m_builder->Generate(Y_Min, Y_Max, xStart, zStart);



   //watch.Stop();
   //Logger.Log("Generated from {0} to {1}: " + DebugTimer.Elapsed(), Y_Min, Y_Max);
   //Logger.Log("Generated surface blocks: {0}", col_data.surfaceBlocksCount);
}

void Column::Serialize()
{
}

LOD_Mode Column::Deserialize(LOD_Mode load_mode)
{
   return LOD_Mode();
}

void Column::CalculateVariables()
{
   m_Current_Mode = LOD_Mode_Empty;
   m_Max_Mode = LOD_Mode_Empty;
   m_Depth = 10;
   m_ChunkSizeX = (int)(m_ChunkMeterSizeX * m_VoxelsPerMeter);
   m_ChunkSizeY = (int)(m_ChunkMeterSizeY * m_VoxelsPerMeter);
   m_ChunkSizeZ = (int)(m_ChunkMeterSizeZ * m_VoxelsPerMeter);
   m_half = ((1.0f / (float)m_VoxelsPerMeter) / 2.0f);
   m_xSideLength = m_ChunkMeterSizeX / (float)m_ChunkSizeX;
   m_ySideLength = m_ChunkMeterSizeY / (float)m_ChunkSizeY;
   m_zSideLength = m_ChunkMeterSizeZ / (float)m_ChunkSizeZ;
   //m_skipDist = Mathf.RoundToInt(1 / (float)m_VoxelsPerMeter);
   //m_locOffset =
   
   m_locOffset[0] = Vector3(0, 0, m_zSideLength);
   m_locOffset[1] = Vector3(m_xSideLength, 0, m_zSideLength);
   m_locOffset[2] = Vector3(m_xSideLength, 0, 0);
   m_locOffset[3] = Vector3(0, 0, 0);
   m_locOffset[4] = Vector3(0, m_ySideLength, m_zSideLength);
   m_locOffset[5] = Vector3(m_xSideLength, m_ySideLength, m_zSideLength);
   m_locOffset[6] = Vector3(m_xSideLength, m_ySideLength, 0);
   m_locOffset[7] = Vector3(0, m_ySideLength, 0);
   
   //globalOffsets = new Vector3Int[8]
   //{
   m_globalOffsets[0] = Vector3Int(0, 0, 1) * m_skipDist;
   m_globalOffsets[0] = Vector3Int(1, 0, 1) * m_skipDist;
   m_globalOffsets[0] = Vector3Int(1, 0, 0) * m_skipDist;
   m_globalOffsets[0] = Vector3Int(0, 0, 0) * m_skipDist;
   m_globalOffsets[0] = Vector3Int(0, 1, 1) * m_skipDist;
   m_globalOffsets[0] = Vector3Int(1, 1, 1) * m_skipDist;
   m_globalOffsets[0] = Vector3Int(1, 1, 0) * m_skipDist;
   m_globalOffsets[0] = Vector3Int(0, 1, 0) * m_skipDist;
   //};
   //AllocateBlockArray(ChunkSizeX, ChunkSizeY, ChunkSizeZ);
   m_Initialized = true;
}