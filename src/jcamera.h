#pragma once
#include "jmath.h"

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