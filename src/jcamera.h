#pragma once
// #include "jlib.h"
#include "jmath.h"
// #include <cmath>
// #include <iostream>
// #include <math.h>
// #include <vector>
// using namespace std;

struct JCamera {
  Vec3 positon;
  Vec3 target;
};

JCamera MakeCamera(Vec3 positon, Vec3 target) {
  JCamera cam;
  cam.positon = positon;
  cam.target = target;
  return cam;
}