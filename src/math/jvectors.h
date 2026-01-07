#pragma once
#include "../consts.h"
#include "../jlib.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>

#include "raylib.h"

using namespace std;

// TODO implement operator overloading
struct Vec2 {
  f32 x;
  f32 y;

  Vec2 operator+(const Vec2 &v2) { return Vec2{x + v2.x, y + v2.y}; }
  Vec2 operator-(const Vec2 &v2) { return Vec2{x - v2.x, y - v2.y}; }
  Vec2 operator*(const Vec2 &v2) { return Vec2{x * v2.x, y * v2.y}; }
  bool operator==(const Vec2 &v2) { return x == v2.x && y == v2.y; };
  // bool operator<(const Vec2 &v2) { return x < v2.x && y < v2.y; };
  // bool operator>(const Vec2 &v2) { return x > v2.x && y > v2.y; };
  explicit operator Vector2() const {
    return Vector2{x, y};
  }

  const char *to_string() const {
    char buffer[50]; // Adjust the size as needed
    std::snprintf(buffer, sizeof(buffer), "Vec2(%f, %f)", x, y);
    return std::string(buffer).c_str();
  }
};

struct Vec3 {
  f32 x;
  f32 y;
  f32 z;

  Vec3 operator+(const Vec3 &v2) { return Vec3{x + v2.x, y + v2.y, z + v2.z}; }
  Vec3 operator-(const Vec3 &v2) { return Vec3{x - v2.x, y - v2.y, z - v2.z}; }
  Vec3 operator*(const Vec3 &v2) { return Vec3{x * v2.x, y * v2.y, z * v2.z}; }

  Vec3 normalized() {
    f32 len = std::sqrt(x * x + y * y + z * z);
    if (len == 0.0) {
      return {0, 0, 0};
    }

    return {
      x / len,
      y / len,
      z / len,
    };
  }

  f32 length() const {
    return sqrtf(x * x + y * y + z * z);
  }

  f32 length_squared() const {
    return (x * x + y * y + z * z);
  }

  Vec3 cross(Vec3 v2) {
    return Vec3{
      y * v2.z - z * v2.y,
      z * v2.x - x * v2.z,
      x * v2.y - y * v2.x,
    };
  }

  void floor_xy() {
    x = floor(x);
    y = floor(y);
  }

  f32 dot(Vec3 v2) { return x * v2.x + y * v2.y + z * v2.z; }

  const char *to_string() const {
    char buffer[50]; // Adjust the size as needed
    std::snprintf(buffer, sizeof(buffer), "Vec2(%f, %f, %f)", x, y, z);
    return std::string(buffer).c_str();
  }
};

struct Vec4 {
  f32 x;
  f32 y;
  f32 z;
  f32 w;
};
