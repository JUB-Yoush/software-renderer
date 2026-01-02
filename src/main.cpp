#include "jcamera.h"
#include "jdraw.h"
#include "mesh.h"
#include "jinput.h"
#include "jmath.h"
#include "raylib.h"
#include "texture.h"
#include "zbuffer.h"
#include <iostream>

// #define SCREEN_WIDTH (800)
// #define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Window title"

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(30);

  JCamera camera = make_camera({0, 0, 3}, {0, 0, 1});
  Light light = make_light({0, -1, 0}, 1);
  ZBuffer zbuffer{};
  Vec3 translation = {0, 0, 0};
  Vec3 rotation = {0, 180, 0};
  f32 scale = 1.0;
  i8 render_modes_count = 5;
  i8 render_mode = 4;

  JTexture texture = j_load_image_from_file(ASSETS_PATH "uv_checker_512.png");

  Matrix4x4 projectionMatrix = make_projection_matrix(
    FOV, SCREEN_HEIGHT, SCREEN_WIDTH, NEAR_PLANE, FAR_PLANE);

  JMesh mesh = load_mesh_from_file(ASSETS_PATH "monkey.obj");
  //JMesh mesh = make_rectangle(2, .5, 2);

  while (!WindowShouldClose()) {
    f32 delta = GetFrameTime();
    handle_inputs(translation, rotation, scale, render_mode, render_modes_count,
                  delta);

    Matrix4x4 translation_matrix =
        make_translation_matrix(translation.x, translation.y, translation.z);

    Matrix4x4 rotation_matrix =
        make_rotation_matrix(rotation.x, rotation.y, rotation.z);

    Matrix4x4 scale_matrix = make_scale_matrix(scale, scale, scale);

    auto model_matrix =
        translation_matrix * (rotation_matrix * scale_matrix);

    auto view_matrix = make_view_matrix(camera.positon, camera.target);

    view_matrix = view_matrix * model_matrix;

    apply_transformations(mesh.transformed_vertices, mesh.vertices, view_matrix);

    BeginDrawing();
    ClearBackground(BLACK);
    //DrawFPS(0, 20);

    ClearZBuffer(zbuffer);
    //DrawTriangle({0, 0}, {10, 5}, {5, 10},GREEN);
    DrawTriangle({0, 0}, {5, 10}, {10, 5}, GREEN);
    switch (render_mode) {
      case 0:
        draw_wire_frame(mesh.transformed_vertices, mesh.triangles, projectionMatrix,
                        GREEN, false);
        DrawText("wire + backface", 0, 0, 20, WHITE);
        break;
      case 1:
        draw_wire_frame(mesh.transformed_vertices, mesh.triangles, projectionMatrix,
                        GREEN, true);
        DrawText("wire", 0, 0, 20, WHITE);
        break;
      case 2:
        draw_unlit(mesh.transformed_vertices, mesh.triangles, projectionMatrix,
                   WHITE, zbuffer);
        DrawText("mesh, unlit", 0, 0, 20, WHITE);
        break;

      case 3:
        draw_flat_shaded(mesh.transformed_vertices, mesh.triangles,
                         projectionMatrix, light, WHITE, zbuffer, 0.2, false);
        DrawText("mesh, lit, pixel based", 0, 0, 20, WHITE);
        break;

      case 4:
        draw_flat_shaded(mesh.transformed_vertices, mesh.triangles,
                         projectionMatrix, light, WHITE, zbuffer, 0.2, true);
        DrawText("mesh, lit, triangle based", 0, 0, 20, WHITE);
        break;

      case 5:
        draw_texture_flat_shaded(mesh.transformed_vertices, mesh.triangles, mesh.uvs,
                                 projectionMatrix, light, texture, zbuffer);
        DrawText("textured + lit", 0, 0, 20, WHITE);
        break;
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
