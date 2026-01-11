#pragma once
#include "../helper/jtypes.h"
#include "physics/collision.h"

struct Triangle {
  i32 points[9];

  // Overload the subscript operator
  i32 &operator[](u64 index);

  // const version for read-only access
  const i32 &operator[](u64 index) const;
};

enum DrawMode {
  WIRE,
  WIRE_CULLED,
  FLAT,
  SHADED,
};

struct JMesh {
  //TODO seperate immutable values to be a pointer instead of making a new copy per mesh
  vector<Vec3> transformed_vertices;
  vector<Vec3> transformed_normals;
  vector<Vec3> vertices; // can be shared
  vector<Vec3> normals; // can be shared
  vector<Vec2> uvs; // can be shared
  vector<Triangle> triangles; // can be shared
  Color color = WHITE;
};


struct JTransform {
  Vec3 translation{};
  Vec3 rotation{};
  f32 scale = 1.0f;
};


bool z_compare(Vec3 v1, Vec3 v2);

vector<Vec3> sorted_z_order(vector<Vec3> vec);

JMesh make_rectangle_mesh(f32 x_len, f32 y_len, f32 z_len);

JMesh make_cube();

JMesh make_aabb_mesh(AABB aabb);

Triangle make_triangle_from_obj_points(f32 points[10]);
