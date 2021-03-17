#include <libnoise.ci>
#include <libnoise.module.ci>
#include <libnoise.model.ci>


typedef struct Settings{
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
}Settings;

typedef struct Result {
   float iso;
   uint type;
}Result;


float TerrainSample(Settings s, float x, float y)
{
   Module perlin_mountains = GetModule(x, 0, y);
   perlin_mountains.m_seed = 0;//seed;
   perlin_mountains.m_frequency = 0.5;
   perlin_mountains.m_noiseQuality = s.quality;

   Module mountains = GetModule(x, 0, y);
   mountains.m_seed = s.seed;
   mountains.m_frequency = 0.5;
   mountains.m_lacunarity = 2;
   mountains.m_noiseQuality = s.quality;

   //perlin_mountains = ScalePoint_GetModule(perlin_mountains, float3(2, 1, 2));
   //mountains = ScalePoint_GetModule(mountains, float3(2, 1, 2));


   perlin_mountains = Perlin_GetValue(perlin_mountains);
   //mountains = RidgedMulti_GetValue(mountains);


   //perlin_mountains = ScaleBias_GetValue(perlin_mountains, 1, 0);
   //mountains = ScaleBias_GetValue(mountains, 0.5, 0);

   float blendMountains = perlin_mountains.Value;// +mountains.Value;

   return perlin_mountains.Value; // (blendMountains * 250 + 280) / 3.5;
}

float CaveSample(Settings s, float x, float y, float z)
{
    return -1;
}

float Angle(float3 from, float3 to)
{
   return acos((float)clamp((float)dot(normalize(from), normalize(to)), (float)-1.0, (float)1.0)) * 57.29578;
}

int Get_Flat_Index(Settings s, int x, int y, int z)
{
   return x + s.ChunkSizeX * (y + s.y_height * z);
}

int Get_Flat_Index_2D(Settings s,int x, int y)
{
   return x * (s.ChunkSizeZ + 2) + y;
}

float GetHeight(Settings s, int x, int y)
{
   return TerrainSample(s, (x * (0.003 / s.VoxelsPerMeter)), (y * (0.003 / s.VoxelsPerMeter))) *250 * s.VoxelsPerMeter + 60;
}

float GetSurfaceHeight(Settings s, float* HeightMap, int LocalX, int LocalZ)
{
   //LocalX = clamp(LocalX, -1, ChunkSizeX);
   //LocalZ = clamp(LocalZ, -1, ChunkSizeZ);

   return HeightMap[(LocalX + 1) * (s.ChunkSizeZ + 2) + (LocalZ + 1)];
}

float3 GetPointNormal(Settings s, float* HeightMap, int x, int z)
{
   float val = (float)GetSurfaceHeight(s, HeightMap, x, z);

   float nx = (val - (float)GetSurfaceHeight(s, HeightMap, x + 1, z)) - (val - (float)GetSurfaceHeight(s, HeightMap, x - 1, z));
   float ny = (val - (float)GetSurfaceHeight(s, HeightMap, x, z) + 1) - (val - (float)GetSurfaceHeight(s, HeightMap, x, z) - 1);
   float nz = (val - (float)GetSurfaceHeight(s, HeightMap, x, z + 1)) - (val - (float)GetSurfaceHeight(s, HeightMap, x, z - 1));

   return normalize((float3)(nx, ny, nz));
}

float Noise(Settings s, float x, float y, float z, float scale, float height, float power)
{
   float rValue = 0;

   rValue = CaveSample(s, x / scale, y / scale, z / scale);
   rValue *= height;

   if (power != 0)
   {
      rValue = pow(rValue, power);
   }

   return rValue;
}

Result GetIsoValue(Settings s, float* HeightMap, int3 localPosition, int3 globalLocation)
{
   Result res;

   res.iso = -1;
   res.type = 1;

   float surfaceHeight = GetSurfaceHeight(s, HeightMap, localPosition.x, localPosition.z);
   res.iso = surfaceHeight - (globalLocation.y * s.VoxelsPerMeter);
   //bool surface = (res.iso > 0);

   if (globalLocation.y < surfaceHeight - 6)
   {
      res.type = 3;
   }
   else if (globalLocation.y < surfaceHeight - 2)
   {
      res.type = 2;
   }

   if (res.type == 1)
   {
      float3 norm = GetPointNormal(s, HeightMap, localPosition.x, localPosition.z);
      //if (Vector3.Distance(globalLocation, new Vector3(globalLocation.x, (float)surfaceHeight, globalLocation.z)) < 1)
      //    Debug.DrawRay(globalLocation, norm, Color.red, 100000);

      if (Angle((float3)(0, 1, 0), norm) > 40)
      {
         res.type = 2; // dirt
      }

      if (Angle((float3)(0, 1, 0), norm) > 50)
      {
         res.type = 3; // rock
      }

   }

   if (s.enableCaves)
   {
      float noiseVal = Noise(s, globalLocation.x, globalLocation.y, globalLocation.z, 16.0,
         17.0, 1.0);
      if (noiseVal > s.caveDensity)
      {
         res.iso = clamp(res.iso, (float)0, (float)1) - noiseVal;
         //surface = false;
      }
   }

   return res;
}

__kernel void CS_Heightmap(__constant Settings *p_settings, __global float *HeightMap)
{
    Settings settings = p_settings[0];
    
    size_t id_x = get_global_id(0);
    size_t id_z = get_global_id(1);
    
    int n_x = settings.noiseX + id_x;
    int n_z = settings.noiseZ + id_z;
    
    float val = GetHeight(settings, n_x, n_z);
    
    HeightMap[Get_Flat_Index_2D(settings, id_x, id_z)] = val;
}

__kernel void hello(__global float *input, __global float *output)
{
  size_t id = get_global_id(0);
  output[id] = id;
}

