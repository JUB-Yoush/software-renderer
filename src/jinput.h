#pragma once
#include "game.h"
#include "jlib.h"
#include "math/calc.h"
#include "raylib.h"

void handle_player_inputs(Vec3 &translation, Vec3 &rotation, f32 &scale,
                          f32 delta_time) {
  // f32 linear_step = (IsKeyDown(KEY_LEFT_SHIFT) ? 0.25 : 1) * delta_time;
  // f32 angular_step = (IsKeyDown(KEY_LEFT_SHIFT) ? 12 : 48) * delta_time;
  f32 speed = 2;

  if (IsKeyDown(KEY_W))
    translation.z += speed;
  if (IsKeyDown(KEY_S))
    translation.z -= speed;
  if (IsKeyDown(KEY_A))
    translation.x += speed;
  if (IsKeyDown(KEY_D))
    translation.x -= speed;
  // if (IsKeyDown(KEY_E))
  //   translation.y += sp;
  // if (IsKeyDown(KEY_Q))
  //   translation.y -= linear_step;

  // if (IsKeyDown(KEY_J))
  //   rotation.x -= angular_step;
  // if (IsKeyDown(KEY_L))
  //   rotation.x += angular_step;
  // if (IsKeyDown(KEY_O))
  //   rotation.y += angular_step;
  // if (IsKeyDown(KEY_U))
  //   rotation.y -= angular_step;
  // if (IsKeyDown(KEY_I))
  //   rotation.z += angular_step;
  // if (IsKeyDown(KEY_K))
  //   rotation.z -= angular_step;
  //
  // if (IsKeyDown(KEY_N))
  //   scale += linear_step;
  // if (IsKeyDown(KEY_M))
  //   scale -= linear_step;
}

void handle_inputs(Vec3 &translation, Vec3 &rotation, f32 &scale,
                   f32 delta_time) {
  f32 linear_step = (IsKeyDown(KEY_LEFT_SHIFT) ? 0.25 : 1) * delta_time;
  f32 angular_step = (IsKeyDown(KEY_LEFT_SHIFT) ? 12 : 48) * delta_time;

  if (IsKeyDown(KEY_W))
    translation.z += linear_step;
  if (IsKeyDown(KEY_S))
    translation.z -= linear_step;
  if (IsKeyDown(KEY_A))
    translation.x += linear_step;
  if (IsKeyDown(KEY_D))
    translation.x -= linear_step;
  if (IsKeyDown(KEY_E))
    translation.y += linear_step;
  if (IsKeyDown(KEY_Q))
    translation.y -= linear_step;

  if (IsKeyDown(KEY_J))
    rotation.x -= angular_step;
  if (IsKeyDown(KEY_L))
    rotation.x += angular_step;
  if (IsKeyDown(KEY_O))
    rotation.y += angular_step;
  if (IsKeyDown(KEY_U))
    rotation.y -= angular_step;
  if (IsKeyDown(KEY_I))
    rotation.z += angular_step;
  if (IsKeyDown(KEY_K))
    rotation.z -= angular_step;

  if (IsKeyDown(KEY_N))
    scale += linear_step;
  if (IsKeyDown(KEY_M))
    scale -= linear_step;
}

void update_render_mode(u8 &render_mode, u8 render_modes_count) {
  if (IsKeyPressed(KEY_LEFT)) {
    render_mode = (render_mode + render_modes_count - 1) % render_modes_count;
  } else if (IsKeyPressed(KEY_RIGHT)) {
    render_mode = (render_mode + 1) % render_modes_count;
  }
}
