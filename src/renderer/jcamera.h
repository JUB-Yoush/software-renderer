#pragma once
#include "../math/calc.h"
#include "ecs/World.h"
#include "math/matrix.h"

struct JCamera {
  Vec3 positon;
  Vec3 rotation;
  Vec3 target;
  Matrix4x4 projection_matrix;
  Matrix4x4 view_matrix;
};

JCamera make_camera(Vec3 positon, Vec3 target) {
  JCamera cam;
  cam.positon = positon;
  cam.target = target;
  cam.projection_matrix = make_projection_matrix(
    FOV, SCREEN_HEIGHT, SCREEN_WIDTH, NEAR_PLANE, FAR_PLANE);

  return cam;
}

void setup_camera(JCamera *cam, Vec3 positon, Vec3 target) {
  cam->positon = positon;
  cam->target = target;
}

