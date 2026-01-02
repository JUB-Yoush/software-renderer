#pragma once
#include "../math/calc.h"
#include "ecs/World.h"

struct JCamera {
  Vec3 positon;
  Vec3 target;
};

JCamera make_camera(Vec3 positon, Vec3 target) {
  JCamera cam;
  cam.positon = positon;
  cam.target = target;
  return cam;
}

void setup_camera(JCamera *cam, Vec3 positon, Vec3 target) {
  cam->positon = positon;
  cam->target = target;
}
