#pragma once
#include "jlib.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

const f32 DEG_TO_RAD = 0.01745329251;
const i32 SCREEN_WIDTH = 800;
const i32 SCREEN_HEIGHT = 600;

const f32 FOV = 70;
const f32 NEAR_PLANE = 1.0;
const f32 FAR_PLANE = 100.0;

// TODO implement operator overloading
struct Vec3 {
  f32 x;
  f32 y;
  f32 z;

  Vec3 operator+(const Vec3 &v2) { return Vec3{x + v2.x, y + v2.y, z + v2.z}; }
  Vec3 operator-(const Vec3 &v2) { return Vec3{x - v2.x, y - v2.y, z - v2.z}; }
  Vec3 operator*(const Vec3 &v2) { return Vec3{x * v2.x, y * v2.y, z * v2.z}; }
};

struct Vec2 {
  f32 x;
  f32 y;
};

struct Vec4 {
  f32 x;
  f32 y;
  f32 z;
  f32 w;
};

Vec3 V3Normalize(Vec3 v) {
  f32 len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  if (len == 0.0) {
    return Vec3{0, 0, 0};
  }

  return Vec3{
      v.x / len,
      v.y / len,
      v.z / len,
  };
}

Vec3 V3CrossProduct(Vec3 v1, Vec3 v2) {
  return Vec3{
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x,
  };
}

f32 V3DotProduct(Vec3 v1, Vec3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

struct Matrix4x4 {
  f32 mat[4][4];

  Matrix4x4() : mat{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}} {};

  Matrix4x4(const f32 val[4][4]) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        mat[i][j] = val[i][j];
      };
    };
  }

  f32 *operator[](i32 i) { return mat[i]; }

  const f32 *operator[](i32 i) const { return mat[i]; }
};

Vec3 Mat4MulVec3(Matrix4x4 mat, Vec3 vec) {
  return Vec3{
      .x =
          mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3],
      .y =
          mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3],
      .z =
          mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3],
  };
}

Vec4 Mat4MulVec4(Matrix4x4 mat, Vec4 vec) {
  return Vec4{
      .x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z +
           mat[0][3] * vec.w,
      .y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z +
           mat[1][3] * vec.w,
      .z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z +
           mat[2][3] * vec.w,
      .w = mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z +
           mat[3][3] * vec.w,
  };
}

Matrix4x4 Mat4Mul(Matrix4x4 a, Matrix4x4 b) {
  Matrix4x4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.mat[i][j] = a.mat[i][0] * b.mat[0][j] + a.mat[i][1] * b.mat[1][j] +
                         a.mat[i][2] * b.mat[2][j] + a.mat[i][3] * b.mat[3][j];
    };
  };
  return result;
}

Matrix4x4 MakeTranslationMatrix(f32 x, f32 y, f32 z) {
  f32 values[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {x, y, z, 1}};
  return Matrix4x4(values);
}

Matrix4x4 MakeScaleMatrix(f32 sx, f32 sy, f32 sz) {
  f32 values[4][4] = {
      {sx, 0, 0, 0}, {0, sy, 0, 0}, {0, 0, sz, 0}, {0, 0, 0, 1}};
  return Matrix4x4(values);
}

Matrix4x4 MakeRotationMatrix(f32 pitch, f32 yaw, f32 roll) {
  f32 alpha = yaw * DEG_TO_RAD;
  f32 beta = pitch * DEG_TO_RAD;
  f32 gamma = roll * DEG_TO_RAD;

  f32 ca = cos(alpha);
  f32 sa = sin(alpha);

  f32 cb = cos(beta);
  f32 sb = sin(beta);

  f32 cg = cos(gamma);
  f32 sg = sin(gamma);

  f32 values[4][4] = {
      {ca * cb, ca * sb * sg - sa * cg, ca * sb * cg + sa * sg, 0.0},
      {sa * cb, sa * sb * sg + ca * cg, sa * sb * cg - ca * sg, 0.0},
      {-sb, cb * sg, cb * cg, 0.0},
      {0.0, 0.0, 0.0, 1.0}};
  return Matrix4x4(values);
};

Matrix4x4 MakeViewMatrix(Vec3 eye, Vec3 target) {
  Vec3 forward = V3Normalize(eye - target);
  Vec3 right = V3CrossProduct(Vec3{0, 1, 0}, forward);
  Vec3 up = V3CrossProduct(forward, right);
  f32 values[4][4] = {
      {right.x, right.y, right.z, -V3DotProduct(right, eye)},
      {up.x, up.y, up.z, -V3DotProduct(up, eye)},
      {forward.x, forward.y, forward.z, -V3DotProduct(forward, eye)},
      {0, 0, 0, 1}};
  return Matrix4x4(values);
};

Matrix4x4 MakeProjectionMatrix(f32 fov, i32 screenHeight, i32 screenWidth,
                               f32 near, f32 far) {
  f32 f = 1.0 / std::tan(fov * 0.5 * DEG_TO_RAD);
  f32 aspect = f32(screenWidth) / f32(screenHeight);

  f32 values[4][4] = {{f / aspect, 0, 0, 0},
                      {0, f, 0, 0},
                      {0, 0, -far / (far - near), -1},
                      {0, 0, -far * near / (far - near), 0}};
  return Matrix4x4(values);
}
