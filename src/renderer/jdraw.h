#pragma once
#include "../consts.h"
#include "mesh.h"
#include "../jlib.h"
#include "../math/calc.h"
#include "../math/jvectors.h"
#include "light.h"
#include "raylib.h"
#include "../math/sort.h"
#include "texture.h"
#include "zbuffer.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

#include "raymath.h"
#include "fmt/xchar.h"
#include "math/matrix.h"
using namespace std;

void apply_transformations(vector<Vec3> &transformed, vector<Vec3> original,
                           Matrix4x4 mat) {
  for (i32 i = 0; i < original.size(); i++) {
    transformed[i] = mat * original[i];
  }
}

void j_draw_line(Vec2 a, Vec2 b, Color color) {
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

bool is_back_face(Vec3 v1, Vec3 v2, Vec3 v3) {
  Vec3 edge1 = v2 - v1;
  Vec3 edge2 = v3 - v1;
  Vec3 cross = edge1.cross(edge2);
  Vec3 cross_norm = cross.normalized();
  Vec3 to_camera = v1.normalized();

  return cross_norm.dot(to_camera) >= 0;
}

f32 get_back_face_value(Vec3 v1, Vec3 v2, Vec3 v3) {
  Vec3 edge1 = v2 - v1;
  Vec3 edge2 = v3 - v1;
  Vec3 cross = edge1.cross(edge2);
  Vec3 cross_norm = cross.normalized();
  Vec3 to_camera = v1.normalized();

  return cross_norm.dot(to_camera);
}

bool is_face_outside_frustrum(Vec3 p1, Vec3 p2, Vec3 p3) {
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

Vec3 project_to_screen(Matrix4x4 mat, Vec3 p) {
  Vec4 clip = Vec4{p.x, p.y, p.z, 1};
  clip = (mat * clip);
  f32 inv_w = 1.0 / clip.w;
  f32 ndc_x = clip.x * inv_w;
  f32 ndc_y = clip.y * inv_w;
  f32 screen_x = (ndc_x * 0.5 + 0.5) * SCREEN_WIDTH;
  f32 screen_y = (ndc_y * 0.5 + 0.5) * SCREEN_HEIGHT;

  return Vec3{screen_x, screen_y, inv_w};
}

void draw_wire_frame(const vector<Vec3> &vertices, vector<Triangle> &triangles,
                     const Matrix4x4 &proj_mat, Color color, bool cull_back_face, bool filled = false

) {
  for (Triangle &tri: triangles) {
    Vec3 v1 = vertices[tri[0]];
    Vec3 v2 = vertices[tri[1]];
    Vec3 v3 = vertices[tri[2]];

    if (cull_back_face && is_back_face(v1, v2, v3)) {
      continue;
    }

    Vec3 p1 = project_to_screen(proj_mat, v1);
    Vec3 p2 = project_to_screen(proj_mat, v2);
    Vec3 p3 = project_to_screen(proj_mat, v3);

    if (is_face_outside_frustrum(p1, p2, p3)) {
      continue;
    }
    if (filled) {
      DrawTriangle({p1.x, p1.y}, {p3.x, p3.y}, {p2.x, p2.y}, color);
    } else {
      DrawTriangleLines(Vector2{p1.x, p1.y}, Vector2{p2.x, p2.y}, Vector2{p3.x, p3.y}, color);
    }
  }
}

bool is_point_outside_viewport(i32 x, i32 y) {
  return x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT;
}

void j_draw_pixel(f32 x, f32 y, Vec3 p1, Vec3 p2, Vec3 p3, Color color,
                  ZBuffer &zbuffer) {
  i32 ix = static_cast<i32>(x);
  i32 iy = static_cast<i32>(y);

  if (is_point_outside_viewport(ix, iy)) {
    return;
  }

  Vec2 p = {x, y};
  Vec3 weights =
      barycentric_weights({p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y}, p);
  f32 alpha = weights.x;
  f32 beta = weights.y;
  f32 gamma = weights.z;

  f32 denom = alpha * p1.z + beta * p2.z + gamma * p3.z;
  f32 depth = 1.0 / denom;

  i32 zindex = SCREEN_WIDTH * iy + ix;
  f32 buffer_value = zbuffer.buff[zindex];

  if (depth < buffer_value) {
    DrawPixel(ix, iy, color);
    zbuffer.buff[zindex] = depth;
  }
}

void draw_filled_triangle(Vec3 &p1, Vec3 &p2, Vec3 &p3, Color color,
                          ZBuffer &zbuffer) {
  // simple FTFB rasterizer
  sort_points(p1, p2, p3);

  p1.floor_xy();
  p2.floor_xy();
  p3.floor_xy();

  // flat bottom
  if (p2.y != p1.y) {
    f32 inverse_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
    f32 inverse_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

    for (i32 y = p1.y; y <= p2.y; y++) {
      f32 x_start = p1.x + (y - p1.y) * inverse_slope1;
      f32 x_end = p1.x + (y - p1.y) * inverse_slope2;

      if (x_start > x_end) {
        swap(x_start, x_end);
      }

      for (i32 x = x_start; x <= x_end; x += 1) {
        j_draw_pixel(x, y, p1, p2, p3, color, zbuffer);
      }
    }
  }

  // flat top
  if (p3.y != p1.y &&
      p3.y != p2.y) {
    // odin impl only checks p3.y != p1.y, no idea why
    f32 inv_slope1 = (p3.x - p2.x) / (p3.y - p2.y);
    f32 inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

    for (i32 y = p2.y; y <= p3.y; y++) {
      f32 x_start = p2.x + (y - p2.y) * inv_slope1;
      f32 x_end = p1.x + (y - p1.y) * inv_slope2;

      if (x_start > x_end) {
        swap(x_start, x_end);
      }

      for (i32 x = x_start; x <= x_end; x += 1) {
        j_draw_pixel(x, y, p1, p2, p3, color, zbuffer);
      }
    }
  }
}

void draw_unlit(const vector<Vec3> &vertices, vector<Triangle> &triangles,
                const Matrix4x4 &proj_mat, Color color, ZBuffer &zbuffer) {
  for (Triangle &tri: triangles) {
    Vec3 v1 = vertices[tri[0]];
    Vec3 v2 = vertices[tri[1]];
    Vec3 v3 = vertices[tri[2]];

    if (is_back_face(v1, v2, v3)) {
      continue;
    }

    Vec3 p1 = project_to_screen(proj_mat, v1);
    Vec3 p2 = project_to_screen(proj_mat, v2);
    Vec3 p3 = project_to_screen(proj_mat, v3);

    if (is_face_outside_frustrum(p1, p2, p3)) {
      continue;
    }
    //DrawTriangle(Vector2{p1.x, p1.y}, Vector2{p2.x, p2.y},Vector2{p3.x, p3.y}, WHITE);
    draw_filled_triangle(p1, p2, p3, color, zbuffer);
  }
}

void draw_flat_shaded(vector<Vec3> &vertices, vector<Triangle> &triangles,
                      Matrix4x4 proj_mat, Light light, Color color,
                      ZBuffer &zbuffer, f32 ambient = 0.2, bool render_trianges = false) {
  auto sorted_tris = triangles;
  for (Triangle &tri: triangles) {
    Vec3 v1 = vertices[tri[0]];
    Vec3 v2 = vertices[tri[1]];
    Vec3 v3 = vertices[tri[2]];

    Vec3 cross = (v2 - v1).cross(v3 - v1);
    Vec3 cross_norm = cross.normalized();
    Vec3 to_cam = v1.normalized();

    if (cross_norm.dot(to_cam) >= 0.0) {
      continue;
    }

    Vec3 p1 = project_to_screen(proj_mat, v1);
    Vec3 p2 = project_to_screen(proj_mat, v2);
    Vec3 p3 = project_to_screen(proj_mat, v3);

    if (is_face_outside_frustrum(p1, p2, p3)) {
      continue;
    }

    f32 intesnity = std::clamp(cross_norm.dot(light.direction), ambient, 1.0f);
    auto shadedColor = Color{
      static_cast<u8>(color.r * intesnity),
      static_cast<u8>(color.g * intesnity),
      static_cast<u8>(color.b * intesnity), color.a
    };
    if (render_trianges) {
      //sort_points(p1, p2, p3);

      // p1.floor_xy();
      // p2.floor_xy();
      // p3.floor_xy();

      // vector<Vec2> vecs = ccw_order({p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y});
      // DrawTriangle(Vector2(vecs[2]), Vector2(vecs[1]), Vector2(vecs[0]), shadedColor);
      DrawTriangle({p3.x, p3.y}, {p2.x, p2.y}, {p1.x, p1.y}, shadedColor);
    } else {
      draw_filled_triangle(p1, p2, p3, shadedColor, zbuffer);
    }
  }
}

void draw_texel_flat_shaded(f32 x, f32 y, Vec3 p1, Vec3 p2, Vec3 p3, Vec2 uv1,
                            Vec2 uv2, Vec2 uv3, JTexture &texture, f32 intensity,
                            ZBuffer &zbuffer) {
  auto ix = static_cast<i32>(x);
  auto iy = static_cast<i32>(y);

  if (is_point_outside_viewport(ix, iy)) {
    return;
  }

  Vec2 p = {x, y};
  Vec3 weights =
      barycentric_weights({p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y}, p);
  f32 alpha = weights.x;
  f32 beta = weights.y;
  f32 gamma = weights.z;

  f32 denom = alpha * p1.z + beta * p2.z + gamma * p3.z;
  f32 depth = 1.0 / denom;

  i32 zindex = SCREEN_WIDTH * iy + ix;
  f32 buffer_value = zbuffer.buff[zindex];

  if (depth <= buffer_value) {
    f32 interpU = ((uv1.x * p1.z) * alpha + (uv2.x * p2.z) * beta +
                   (uv3.x * p3.z) * gamma) *
                  depth;
    f32 interpV = ((uv1.y * p1.z) * alpha + (uv2.y * p2.z) * beta +
                   (uv3.y * p3.z) * gamma) *
                  depth;

    auto texX = i32(interpU * f32(texture.width)) % texture.width;
    auto texY = i32(interpV * f32(texture.height)) % texture.height;
    auto tex = texture.pixels[texY * texture.width + texX];

    auto shadedTex =
        Color{
          u8(f32(tex.r) * intensity), u8(f32(tex.g) * intensity),
          u8(f32(tex.b) * intensity), tex.a
        };
    DrawPixel(ix, iy, shadedTex);
    zbuffer.buff[zindex] = depth;
  }
}

void draw_textured_triangle_flat_shaded(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec2 &uv1,
                                        Vec2 &uv2, Vec2 &uv3, JTexture &texture,
                                        f32 intensity, ZBuffer &zbuffer) {
  // simple FTFB rasterizer
  sort_points(p1, p2, p3);

  p1.floor_xy();
  p2.floor_xy();
  p3.floor_xy();

  // flat bottom
  if (p2.y != p1.y) {
    f32 inverse_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
    f32 inverse_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

    for (i32 y = p1.y; y <= p2.y; y++) {
      f32 x_start = p1.x + (y - p1.y) * inverse_slope1;
      f32 x_end = p1.x + (y - p1.y) * inverse_slope2;

      if (x_start > x_end) {
        swap(x_start, x_end);
      }

      for (i32 x = x_start; x <= x_end; x += 1) {
        draw_texel_flat_shaded(x, y, p1, p2, p3, uv1, uv2, uv3, texture, intensity,
                               zbuffer);
      }
    }
  }

  // flat top
  if (p3.y != p1.y &&
      p3.y != p2.y) {
    // odin impl only checks p3.y != p1.y, no idea why
    f32 inv_slope1 = (p3.x - p2.x) / (p3.y - p2.y);
    f32 inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);

    for (i32 y = p2.y; y <= p3.y; y++) {
      f32 x_start = p2.x + (y - p2.y) * inv_slope1;
      f32 x_end = p1.x + (y - p1.y) * inv_slope2;

      if (x_start > x_end) {
        swap(x_start, x_end);
      }

      for (i32 x = x_start; x <= x_end; x += 1) {
        draw_texel_flat_shaded(x, y, p1, p2, p3, uv1, uv2, uv3, texture, intensity,
                               zbuffer);
        // JDrawPixel(x, y, p1, p2, p3, color, zbuffer);
      }
    }
  }
}

void draw_texture_flat_shaded(vector<Vec3> &vertices, vector<Triangle> &triangles,
                              vector<Vec2> &uvs, Matrix4x4 proj_mat, Light light,
                              JTexture texture, ZBuffer &zbuffer,
                              f32 ambient = 0.2) {
  for (Triangle &tri: triangles) {
    Vec3 v1 = vertices[tri[0]];
    Vec3 v2 = vertices[tri[1]];
    Vec3 v3 = vertices[tri[2]];

    Vec2 uv1 = uvs[tri[3]];
    Vec2 uv2 = uvs[tri[4]];
    Vec2 uv3 = uvs[tri[5]];

    Vec3 cross = (v2 - v1).cross(v3 - v1);
    Vec3 cross_norm = cross.normalized();
    Vec3 to_cam = v1.normalized();

    if (cross_norm.dot(to_cam) >= 0.0) {
      continue;
    }

    Vec3 p1 = project_to_screen(proj_mat, v1);
    Vec3 p2 = project_to_screen(proj_mat, v2);
    Vec3 p3 = project_to_screen(proj_mat, v3);

    if (is_face_outside_frustrum(p1, p2, p3)) {
      continue;
    }

    f32 intesnity = std::clamp(cross_norm.dot(light.direction), ambient, 1.0f);
    draw_textured_triangle_flat_shaded(p1, p2, p3, uv1, uv2, uv3, texture,
                                       intesnity, zbuffer);
  }
}

void draw_texture_unlit(vector<Vec3> &vertices, vector<Triangle> &triangles,
                        vector<Vec2> &uvs, Matrix4x4 proj_mat, JTexture texture,
                        Color color, ZBuffer &zbuffer) {
  for (Triangle &tri: triangles) {
    Vec3 v1 = vertices[tri[0]];
    Vec3 v2 = vertices[tri[1]];
    Vec3 v3 = vertices[tri[2]];

    Vec2 uv1 = uvs[tri[3]];
    Vec2 uv2 = uvs[tri[4]];
    Vec2 uv3 = uvs[tri[5]];

    if (is_back_face(v1, v2, v3)) {
      continue;
    }

    Vec3 p1 = project_to_screen(proj_mat, v1);
    Vec3 p2 = project_to_screen(proj_mat, v2);
    Vec3 p3 = project_to_screen(proj_mat, v3);

    if (is_face_outside_frustrum(p1, p2, p3)) {
      continue;
    }
    // DrawFilledTriangle(p1, p2, p3, color, zbuffer);
    draw_textured_triangle_flat_shaded(p1, p2, p3, uv1, uv2, uv3, texture,
                                       1.0, // Unlit
                                       zbuffer);
  }
}
