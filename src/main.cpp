#include "renderer/jcamera.h"
#include "renderer/jdraw.h"
#include "renderer/mesh.h"
#include "jinput.h"
#include "math/calc.h"
#include "raylib.h"
#include "renderer/texture.h"
#include "renderer/zbuffer.h"
#include "ecs/query.h"
#include "ecs/world.h"
#include <iostream>
#include "game.h"
#include "jinput.h"

// #define SCREEN_WIDTH (800)
// #define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Window title"

void update_models(Game &game) {
  const f32 delta = GetFrameTime();
  for (EntityId ent: Query<JMesh, JTransform>(game.world)) {
    auto &translation = game.world.get<JTransform>(ent)->translation;
    auto &rotation = game.world.get<JTransform>(ent)->rotation;
    auto &scale = game.world.get<JTransform>(ent)->scale;
    auto *mesh = game.world.get<JMesh>(ent);

    Matrix4x4 translation_matrix =
        make_translation_matrix(translation.x, translation.y, translation.z);

    Matrix4x4 rotation_matrix =
        make_rotation_matrix(rotation.x, rotation.y, rotation.z);

    Matrix4x4 scale_matrix = make_scale_matrix(scale, scale, scale);

    auto model_matrix =
        translation_matrix * (rotation_matrix * scale_matrix);

    auto view_model_matrix = game.camera->view_matrix * model_matrix;

    apply_transformations(mesh->transformed_vertices, mesh->vertices, view_model_matrix);
  }
}

void update_control_mode(Game &game) {
  if (IsKeyPressed(KEY_ONE)) {
    game.control_mode = Game::PLAYER;
  }
  if (IsKeyPressed(KEY_TWO)) {
    game.control_mode = Game::CAMERA;
  }
}

void poll_inputs(Game &game, float delta) {
  switch (game.control_mode) {
    case Game::CAMERA: {
      f32 mi_bombo = 1;
      handle_inputs(game.camera->positon, game.camera->rotation, mi_bombo,
                    delta);
      break;
    }

    case Game::PLAYER: {
      handle_inputs(game.playerref.transform->translation, game.playerref.transform->rotation,
                    game.playerref.transform->scale,
                    delta);
      break;
    }
  }
  fmt::print("{}\n", game.camera->positon.x);
  fmt::print("{}\n", game.camera->positon.y);
  fmt::print("{}\n", game.camera->positon.z);
}

void update_camera(Game &game) {
  // game.camera->target = {
  //   .x = game.camera->positon.x,
  //   .y = game.camera->positon.y,
  //   .z = game.camera->positon.z + 1
  // };
  game.camera->view_matrix = make_view_matrix(game.camera->positon, game.camera->target);
  game.camera->target = {game.camera->positon.x, game.camera->positon.y, 0};
}

void update(Game &game) {
  const f32 delta = GetFrameTime();
  update_render_mode(game.render_mode, 5);
  update_control_mode(game);
  poll_inputs(game, delta);
  update_camera(game);
  update_models(game);
}

void draw_models(Game &game) {
  for (EntityId ent: Query<JMesh>(game.world)) {
    auto *mesh = game.world.get<JMesh>(ent);
    // switch (game.render_mode) {
    //   case 0:
    //     draw_wire_frame(mesh->transformed_vertices, mesh->triangles, game.camera->projection_matrix,
    //                     GREEN, false);
    //     DrawText("wire + backface", 0, 0, 20, WHITE);
    //     break;
    //   case 1:
    //     draw_wire_frame(mesh->transformed_vertices, mesh->triangles, game.camera->projection_matrix,
    //                     GREEN, true);
    //     DrawText("wire", 0, 0, 20, WHITE);
    //     break;
    //   case 2:
    //     draw_unlit(mesh->transformed_vertices, mesh->triangles, game.camera->projection_matrix,
    //                WHITE, game.zbuffer);
    //     DrawText("mesh, unlit", 0, 0, 20, WHITE);
    //     break;
    //
    //   case 3:
    //     draw_flat_shaded(mesh->transformed_vertices, mesh->triangles,
    //                      game.camera->projection_matrix, *game.light, WHITE, game.zbuffer, 0.2, false);
    //     DrawText("mesh, lit, pixel based", 0, 0, 20, WHITE);
    //     break;
    //
    //   case 4:
    draw_flat_shaded(mesh->transformed_vertices, mesh->triangles,
                     game.camera->projection_matrix, *game.light, mesh->color, 0.2, true);
    DrawText("mesh, lit, triangle based", 0, 10, 10, WHITE);
    //DrawText(, 0, 10, 10, WHITE);
    //   break;

    // case 5:
    //   draw_texture_flat_shaded(mesh.transformed_vertices, mesh.triangles, mesh.uvs,
    //                            projectionMatrix, *light, texture, game.zbuffer);
    //   DrawText("textured + lit", 0, 0, 20, WHITE);
    //   break;
    //default: ;
  }
};

void draw(Game &game) {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawFPS(0, 20);
  draw_models(game);
  EndDrawing();
}

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(120);


  Game game;
  EntityId camera_id = game.world.new_entity();
  EntityId light_id = game.world.new_entity();
  EntityId floor = game.world.new_entity();
  auto *floor_mesh = game.world.assign<JMesh>(floor);
  auto *floor_trans = game.world.assign<JTransform>(floor);
  *floor_mesh = make_rectangle(10, .5, 10);
  auto *light = game.world.assign<Light>(camera_id);
  auto *cam = game.world.assign<JCamera>(camera_id);
  game.camera = cam;
  game.light = light;

  setup_camera(game.camera, {0, -2, 8}, {0, 0, 1});
  setup_light(game.light, {0, -1, 0}, 1);

  // setup player
  EntityId player_id = game.world.new_entity();
  auto *player = game.world.assign<Player>(player_id);
  auto *pmesh = game.world.assign<JMesh>(player_id);
  auto *ptransform = game.world.assign<JTransform>(player_id);

  *pmesh = make_rectangle(1, 1, 1);
  pmesh->color = RED;

  game.playerref.transform = ptransform;
  game.playerref.mesh = pmesh;

  while (!WindowShouldClose()) {
    const f32 delta = GetFrameTime();
    //input();
    update(game);
    draw(game);
  }

  CloseWindow();

  return 0;
}

