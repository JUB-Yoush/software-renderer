#include "renderer/camera.h"
#include "renderer/renderer.h"
#include "renderer/mesh.h"
#include "helper/jinput.h"
#include "math/calc.h"
#include "raylib.h"
#include "renderer/texture.h"
#include "renderer/zbuffer.h"
#include "ecs/query.h"
#include "ecs/world.h"
#include <iostream>
#include "game/game.h"
#include "game/player.h"
#include "helper/jinput.h"
#include "physics/collision.h"

// #define SCREEN_WIDTH (800)
// #define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Window title"

void update_models(Game &game) {
  const f32 delta = GetFrameTime();
  for (EntityId ent: Query<JMesh, JTransform, AABB>(game.world)) {
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
      handle_inputs(game.camera->position, game.camera->rotation, mi_bombo,
                    delta);
      break;
    }

    case Game::PLAYER: {
      game.input_dir = Vec2::zero();
      if (IsKeyDown(KEY_W))
        game.input_dir.y = -1;
      if (IsKeyDown(KEY_S))
        game.input_dir.y = 1;
      if (IsKeyDown(KEY_A))
        game.input_dir.x = -1;
      if (IsKeyDown(KEY_D))
        game.input_dir.x = 1;
      game.input_dir = game.input_dir.normalized();
      break;
    }
  }
}

bool player_touching_ground(Game &game) {
  // use it's aabb to check if we're touching ground in x y or z
  auto *p_aabb = game.world.get<AABB>(game.player_id);
  const auto p_translation = game.world.get<JTransform>(game.player_id)->translation;
  for (const EntityId ent: Query<Floor, AABB>(game.world)) {
    const auto aabb = game.world.get<AABB>(ent);
    const auto translation = game.world.get<JTransform>(ent)->translation;
    AABB p_translated_box = p_aabb->translated(p_translation);
    AABB translated_box = aabb->translated(translation);
    if (AABB::has_intersection(p_translated_box, translated_box)) {
      return true;
    }
  }
  return false;
}

void update_player(Game &game, f32 delta) {
  f32 speed = 5;
  f32 gravity = .1;
  bool is_on_floor = player_touching_ground(game);
  if (game.input_dir != Vec2::zero()) {
    *game.playerref.velocity = Velocity{
      game.input_dir.x * speed, game.playerref.velocity->y, game.input_dir.y * speed
    };
  } else {
    *game.playerref.velocity = Velocity{0, game.playerref.velocity->y, 0};
  }
  if (!is_on_floor) {
    game.playerref.velocity->y += gravity;
  } else {
    game.playerref.velocity->y = 0;
  }
  if (IsKeyPressed(KEY_RIGHT_CONTROL) && is_on_floor) {
    game.playerref.velocity->y -= speed;
  }
  game.playerref.transform->translation += *game.playerref.velocity * delta;
}


void update_camera(Game &game) {
  // game.camera->target = {
  //   .x = game.camera->positon.x,
  //   .y = game.camera->positon.y,
  //   .z = game.camera->positon.z + 1
  // };
  game.camera->view_matrix = make_view_matrix(game.camera->position, game.camera->target);
  game.camera->target = {game.camera->position.x, game.camera->position.y, 0};
}

void update(Game &game) {
  const f32 delta = GetFrameTime();
  update_render_mode(game.render_mode, 5);
  update_control_mode(game);
  poll_inputs(game, delta);
  update_player(game, delta);
  update_camera(game);
  update_models(game);
}


void draw_floor(Game &game) {
  for (EntityId ent: Query<Floor>(game.world)) {
    auto *mesh = game.world.get<JMesh>(ent);
    draw_flat_shaded(mesh->transformed_vertices, mesh->triangles,
                     game.camera->projection_matrix, *game.light, mesh->color, 0.2, true);
  }
}

void draw_models(Game &game) {
  vector<EntityId> entities_to_draw{};
  JCamera *cam = game.camera;
  // just store the ids, get the components in the comparison function
  for (EntityId ent: Query<JMesh, JTransform>(game.world)) {
    entities_to_draw.emplace_back(ent);
  }

  /* z depth based mesh sorting
   * I'd need to implment some polygon cliping related algorithm that seperates overlapping parts into seperate polygons that are drawn seperatley.
   * something like https://www.geeksforgeeks.org/dsa/polygon-clipping-sutherland-hodgman-algorithm/
   * for now just not being able to change the y of the camera and always drawing the floor last seems like a fine solution
   */
  ranges::sort(entities_to_draw, [&cam, &game](EntityId a, EntityId b) {
                 // sort decending, furthest elements are drawn first
                 auto *a_trans = game.world.get<JTransform>(a);
                 auto *b_trans = game.world.get<JTransform>(b);
                 f32 a_dist = (a_trans->translation - cam->position).length_squared();
                 f32 b_dist = (b_trans->translation - cam->position).length_squared();
                 return a_dist < b_dist;
               }
  );

  for (const EntityId id: entities_to_draw) {
    if (game.world.has<Floor>(id)) {
      continue;
    }

    // if (game.world.has<Player>(id)) {
    //   DrawText(game.world.get<JTransform>(id)->translation.to_string(), 0, 30, 10, WHITE);
    // }

    auto *mesh = game.world.get<JMesh>(id);
    draw_flat_shaded(mesh->transformed_vertices, mesh->triangles,
                     game.camera->projection_matrix, *game.light, mesh->color, 0.2, true);
    DrawText("mesh, lit, triangle based", 0, 10, 20, WHITE);
  }
};

void draw(Game &game) {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawFPS(0, 0);
  draw_floor(game);
  draw_models(game);
  EndDrawing();
}

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(120);


  Game game;
  JCamera::make_camera(game, {0, -2, 8}, {0, 0, 1});
  Light::make_light(game, {0, -1, 0}, 1);

  // setup player
  Player::make_player(game);
  Game::make_floor(game, {10, .5, 10});

  while (!WindowShouldClose()) {
    const f32 delta = GetFrameTime();
    //input();
    update(game);
    draw(game);
  }

  CloseWindow();

  return 0;
}

