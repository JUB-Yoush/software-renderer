#pragma once
#include "jlib.h"
#include "jmath.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

struct Triangle {
  // 3vert.     3uvs       3norm.
  i32 points[9];

  // Overload the subscript operator
  i32 &operator[](u64 index) {
    if (index >= 0 && index < 9) {
      return points[index];
    }
    throw out_of_range("Index out of range");
  }

  // const version for read-only access
  const i32 &operator[](u64 index) const {
    if (index >= 0 && index < 9) {
      return points[index];
    }
    throw out_of_range("Index out of range");
  }
};

struct JMesh {
  vector<Vec3> transformed_vertices;
  vector<Vec3> transformed_normals;
  vector<Vec3> vertices;
  vector<Vec3> normals;
  vector<Vec2> uvs;
  vector<Triangle> triangles;
};

JMesh MakeCube() {

  vector<Vec3> transformed_verticies;
  transformed_verticies.reserve(8);

  vector<Vec3> transformed_normals;
  transformed_normals.reserve(6);

  vector<Vec3> vertices;
  vertices.reserve(8);
  vertices[0] = Vec3{-1.0, -1.0, -1.0};
  vertices[1] = Vec3{-1.0, 1.0, -1.0};
  vertices[2] = Vec3{1.0, 1.0, -1.0};
  vertices[3] = Vec3{1.0, -1.0, -1.0};
  vertices[4] = Vec3{1.0, 1.0, 1.0};
  vertices[5] = Vec3{1.0, -1.0, 1.0};
  vertices[6] = Vec3{-1.0, 1.0, 1.0};
  vertices[7] = Vec3{-1.0, -1.0, 1.0};

  vector<Vec3> normals;
  normals.reserve(6);
  normals[0] = {0.0, 0.0, -1.0};
  normals[1] = {1.0, 0.0, 0.0};
  normals[2] = {0.0, 0.0, 1.0};
  normals[3] = {-1.0, 0.0, 0.0};
  normals[4] = {0.0, 1.0, 0.0};
  normals[5] = {0.0, -1.0, 0.0};

  vector<Vec2> uvs;
  uvs.reserve(4);
  uvs[0] = Vec2{1.0, 1.0};
  uvs[1] = Vec2{1.0, 0.0};
  uvs[2] = Vec2{0.0, 0.0};
  uvs[3] = Vec2{0.0, 1.0};

  vector<Triangle> triangles;
  triangles.reserve(12);

  // Front                 vert.     uvs       norm.
  triangles[0] = Triangle{0, 1, 2, 0, 1, 2, 0, 0, 0};
  triangles[1] = Triangle{0, 2, 3, 0, 2, 3, 0, 0, 0};
  // Right
  triangles[2] = Triangle{3, 2, 4, 0, 1, 2, 1, 1, 1};
  triangles[3] = Triangle{3, 4, 5, 0, 2, 3, 1, 1, 1};
  // Back
  triangles[4] = Triangle{5, 4, 6, 0, 1, 2, 2, 2, 2};
  triangles[5] = Triangle{5, 6, 7, 0, 2, 3, 2, 2, 2};
  // Left
  triangles[6] = Triangle{7, 6, 1, 0, 1, 2, 3, 3, 3};
  triangles[7] = Triangle{7, 1, 0, 0, 2, 3, 3, 3, 3};
  // Top
  triangles[8] = Triangle{1, 6, 4, 0, 1, 2, 4, 4, 4};
  triangles[9] = Triangle{1, 4, 2, 0, 2, 3, 4, 4, 4};
  // Bottom
  triangles[10] = Triangle{5, 7, 0, 0, 1, 2, 5, 5, 5};
  triangles[11] = Triangle{5, 0, 3, 0, 2, 3, 5, 5, 5};
  return JMesh{
      .transformed_vertices = transformed_verticies,
      .transformed_normals = transformed_normals,
      .vertices = vertices,
      .normals = normals,
      .uvs = uvs,
      .triangles = triangles,
  };
}
