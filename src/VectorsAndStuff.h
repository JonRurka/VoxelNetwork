#include "stdafx.h"

#pragma once
struct Vector3
{
public:
   float x;
   float y;
   float z;

   Vector3()
   {
      x = 0;
      y = 0;
      z = 0;
   }

   Vector3(float p_x, float p_y, float p_z) {
      x = p_x;
      y = p_y;
      z = p_z;
   }


   inline bool operator==(Vector3 second) {
      return (x == second.x && y == second.y && z == second.z);
   }

   inline bool operator!=(Vector3 second) {
      return (x != second.x || y != second.y || z != second.z);
   }

   inline Vector3 operator +(Vector3 second) {
      Vector3 result = Vector3();
      result.x = x + second.x;
      result.y = y + second.y;
      result.z = z + second.z;
      return result;
   }

   inline Vector3 operator*(Vector3 second) {
      Vector3 result = Vector3();
      result.x = x * second.x;
      result.y = y * second.y;
      result.z = z * second.z;
      return result;
   }

   inline Vector3 operator*(int val) {
      Vector3 result = Vector3();
      result.x = x * val;
      result.y = y * val;
      result.z = z * val;
      return result;
   }

   std::string File_String()
   {
      return std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);
   }
};

struct Vector2
{
public:
   float x;
   float y;

   Vector2()
   {
      x = 0;
      y = 0;
   }
   
   Vector2(float p_x, float p_y) {
      x = p_x;
      y = p_y;
   }

   inline bool operator==(Vector3 second) {
      return (x == second.x && y == second.y);
   }
   inline bool operator!=(Vector3 second) {
      return (x != second.x || y != second.y);
   }
   inline Vector3 operator +(Vector3 second) {
      Vector3 result = Vector3();
      result.x = x + second.x;
      result.y = y + second.y;
      return result;
   }
   inline Vector3 operator*(Vector3 second) {
      Vector3 result = Vector3();
      result.x = x * second.x;
      result.y = y * second.y;
      return result;
   }
   inline Vector3 operator*(int val) {
      Vector3 result = Vector3();
      result.x = x * val;
      result.y = y * val;
      return result;
   }

   std::string File_String()
   {
      return std::to_string(x) + "_" + std::to_string(y);
   }
};

struct Vector3Int
{
public:
   int x;
   int y;
   int z;

   Vector3Int()
   {
      x = 0;
      y = 0;
      z = 0;
   }

   Vector3Int(int p_x, int p_y, int p_z) {
      x = p_x;
      y = p_y;
      z = p_z;
   }

   inline bool operator==(Vector3Int second) {
      return (x == second.x && y == second.y && z == second.z);
   }

   inline bool operator!=(Vector3Int second) {
      return (x != second.x || y != second.y || z != second.z);
   }

   inline Vector3Int operator +(Vector3Int second) {
      Vector3Int result = Vector3Int();
      result.x = x + second.x;
      result.y = y + second.y;
      result.z = z + second.z;
      return result;
   }

   inline Vector3Int operator*(Vector3Int second) {
      Vector3Int result = Vector3Int();
      result.x = x * second.x;
      result.y = y * second.y;
      result.z = z * second.z;
      return result;
   }

   inline Vector3Int operator*(int val) {
      Vector3Int result = Vector3Int();
      result.x = x * val;
      result.y = y * val;
      result.z = z * val;
      return result;
   }

   std::string File_String()
   {
      return std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);
   }
};

struct Vector2Int
{
public:
   int x;
   int y;

   Vector2Int()
   {
      x = 0;
      y = 0;
   }

   Vector2Int(int p_x, int p_y) {
      x = p_x;
      y = p_y;
   }

   inline bool operator==(Vector3Int second) {
      return (x == second.x && y == second.y);
   }

   inline bool operator!=(Vector3Int second) {
      return (x != second.x || y != second.y);
   }

   inline Vector3Int operator +(Vector3Int second) {
      Vector3Int result = Vector3Int();
      result.x = x + second.x;
      result.y = y + second.y;
      return result;
   }

   inline Vector3Int operator*(Vector3Int second) {
      Vector3Int result = Vector3Int();
      result.x = x * second.x;
      result.y = y * second.y;
      return result;
   }

   inline Vector3Int operator*(int val) {
      Vector3Int result = Vector3Int();
      result.x = x * val;
      result.y = y * val;
      return result;
   }

   std::string File_String()
   {
      return std::to_string(x) + "_" + std::to_string(y);
   }
};

struct Block
{
public:
   unsigned int type;
   float iso;
   bool set;

   Block(unsigned int p_type)
   {
      type = type;
      iso = 0;
      set = false;
   }

   Block(float p_iso)
   {
      iso = iso;
      type = 0;
      set = false;
   }

   Block(unsigned int type, float iso)
   {
      iso = iso;
      type = type;
      set = false;
   }
};