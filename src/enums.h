#pragma once

typedef unsigned int LOD_Mode;

#define LOD_Mode_Empty           0
#define LOD_Mode_Heightmap       1
#define LOD_Mode_ReducedDepth    2
#define LOD_Mode_Full            3


typedef unsigned int UpdateMode;

#define UpdateMode_EmptyToHeightmap    0
#define UpdateMode_EmptyToReduced      1
#define UpdateMode_EmptyToFull         2
#define UpdateMode_HeightmapToReduced  3
#define UpdateMode_HeightmapToFull     4
#define UpdateMode_ReducedToFull       5
