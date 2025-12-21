#pragma once
#include "consts.h"
#include "jgraphics.h"
#include "jlib.h"
#include "jmath.h"
#include "raylib.h"
#include <cmath>
#include <cstdio>
using namespace std;

void ApplyTransformations(vector<Vec3> &transformed, vector<Vec3> original,
                          Matrix4x4 mat) {
  for (i32 i = 0; i < original.size(); i++) {
    transformed[i] = Mat4MulVec3(mat, original[i]);
  }
}

void JDrawLine(Vec2 a, Vec2 b, Color color) {
  f32 dx = b.x - a.x;
  f32 dy = b.y - a.y;
  f32 longer_delta = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);
  f32 inc_x = dx / longer_delta;
  f32 inc_y = dy / longer_delta;

  f32 x = a.x;
  f32 y = a.y;
  for (i32 i = 0; i <= i32(longer_delta); i++) {
    DrawPixel(i32(x), i32(y), color);
    x += inc_x;
    y += inc_y;
  }
}

bool IsBackFace(Vec3 v1, Vec3 v2, Vec3 v3) {
  Vec3 edge1 = v2 - v1;
  Vec3 edge2 = v3 - v1;
  Vec3 cross = edge1.cross(edge2);
  Vec3 cross_norm = cross.normalized();
  Vec3 to_camera = v1.normalized();

  return cross_norm.dot(to_camera) >= 0;
}

bool IsFaceOutsideFrustrum(Vec3 p1, Vec3 p2, Vec3 p3) {
  if ((p1.z > 1.0 || p2.z > 1.0 || p3.z > 1.0) ||
      (p1.z < -1.0 || p2.z < -1.0 || p3.z < -1.0)) {
    return true;
  }
  f32 min_x = min(p1.x, min(p2.x, p3.x));
  f32 max_x = max(p1.x, max(p2.x, p3.x));
  f32 min_y = min(p1.y, min(p2.y, p3.y));
  f32 max_y = max(p1.y, max(p2.y, p3.y));

  if (max_x < 0 || min_x > SCREEN_WIDTH || max_y < 0 || min_y > SCREEN_HEIGHT) {
    return true;
  }
  return false;
}

Vec3 ProjectToScreen(Matrix4x4 mat, Vec3 p) {
  Vec4 clip = Mat4MulVec4(mat, Vec4{p.x, p.y, p.z, 1});
  f32 inv_w = 1.0 / clip.w;
  f32 ndc_x = clip.x * inv_w;
  f32 ndc_y = clip.y * inv_w;
  f32 screen_x = (ndc_x * 0.5 + 0.5) * SCREEN_WIDTH;
  f32 screen_y = (ndc_y * 0.5 + 0.5) * SCREEN_HEIGHT;

  return Vec3{screen_x, screen_y, inv_w};
}

void DrawWireFrame(vector<Vec3> &vertices, vector<Triangle> &triangles,
                   Matrix4x4 &proj_mat, Color color, bool cull_back_face

) {
  for (Triangle &tri : triangles) {
    Vec3 v1 = vertices[tri[0]];
    Vec3 v2 = vertices[tri[1]];
    Vec3 v3 = vertices[tri[2]];

    if (cull_back_face && IsBackFace(v1, v2, v3)) {
      continue;
    }

    Vec3 p1 = ProjectToScreen(proj_mat, v1);
    Vec3 p2 = ProjectToScreen(proj_mat, v2);
    Vec3 p3 = ProjectToScreen(proj_mat, v3);

    if (IsFaceOutsideFrustrum(p1, p2, p3)) {
      continue;
    }
    JDrawLine(Vec2{p1.x, p1.y}, Vec2{p2.x, p2.y}, color);
    JDrawLine(Vec2{p2.x, p2.y}, Vec2{p3.x, p3.y}, color);
    JDrawLine(Vec2{p3.x, p3.y}, Vec2{p1.x, p1.y}, color);
    // DrawLine(p1.x, p1.y, p2.x, p2.y, color);
    // DrawLine(p2.x, p2.y, p3.x, p3.y, color);
    // DrawLine(p3.x, p3.y, p1.x, p1.y, color);
  }
}