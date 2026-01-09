#pragma once
#include "game.h"
#include "../math/calc.h"
#include "../ecs/world.h"
#include "../math/matrix.h"
struct Game;

struct JCamera {
  Vec3 position;
  Vec3 rotation;
  Vec3 target;
  Matrix4x4 projection_matrix;
  Matrix4x4 view_matrix;

  static void make_camera(Game &game, Vec3 positon, Vec3 target);
};



