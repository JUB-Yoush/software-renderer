#include "jcamera.h"
#include "jdraw.h"
#include "jgraphics.h"
#include "jinput.h"
#include "jmath.h"
#include "raylib.h"
#include "zubffer.h"
#include <iostream>

// #define SCREEN_WIDTH (800)
// #define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Window title"

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);

  // Texture2D texture =
  //     LoadTexture(ASSETS_PATH "test.png"); // Check README.md for how this
  //     works

  JCamera camera = MakeCamera({0, 0, -3}, {0, 0, -1});
  ZBuffer zbuffer;
  Vec3 translation = {0, 0, 0};
  Vec3 rotation = {0, 0, 0};
  f32 scale = 1.0;
  i8 render_modes_count = 3;
  i8 render_mode = render_modes_count - 2;

  Matrix4x4 projectionMatrix = MakeProjectionMatrix(
      FOV, SCREEN_HEIGHT, SCREEN_WIDTH, NEAR_PLANE, FAR_PLANE);

  while (!WindowShouldClose()) {

    f32 delta = GetFrameTime();
    handle_inputs(translation, rotation, scale, render_mode, render_modes_count,
                  delta);

    Matrix4x4 translation_matrix =
        MakeTranslationMatrix(translation.x, translation.y, translation.z);

    Matrix4x4 rotation_matrix =
        MakeRotationMatrix(rotation.x, rotation.y, rotation.z);

    Matrix4x4 scale_matrix = MakeScaleMatrix(scale, scale, scale);

    auto model_matrix =
        Mat4Mul(translation_matrix, Mat4Mul(rotation_matrix, scale_matrix));

    auto view_matrix = MakeViewMatrix(camera.positon, camera.target);

    view_matrix = Mat4Mul(view_matrix, model_matrix);

    JMesh mesh = MakeCube();
    ApplyTransformations(mesh.transformed_vertices, mesh.vertices, view_matrix);

    BeginDrawing();
    ClearBackground(BLACK);

    ClearZBuffer(zbuffer);

    switch (render_mode) {
    case 0:
      DrawWireFrame(mesh.transformed_vertices, mesh.triangles, projectionMatrix,
                    GREEN, false);
      DrawText("mode 1", 0, 0, 20, WHITE);
      break;
    case 1:
      DrawWireFrame(mesh.transformed_vertices, mesh.triangles, projectionMatrix,
                    GREEN, true);
      DrawText("mode 2", 0, 0, 20, WHITE);
      break;
    case 2:
      DrawUnlit(mesh.transformed_vertices, mesh.triangles, projectionMatrix,
                WHITE, zbuffer);
      DrawText("mode 3", 0, 0, 20, WHITE);
      break;
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
