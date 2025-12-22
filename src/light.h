#pragma once

#include "jvectors.h"

struct Light {
  Vec3 direction;
  f32 strength;
};

Light make_light(Vec3 direction, f32 strength) {
  return {
      direction.normalized(),
      strength,
  };
}