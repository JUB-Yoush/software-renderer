#pragma once
#include "../helper/consts.h"
#include "../helper/jlib.h"
#include "vectors.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>


Vec3 barycentric_weights(Vec2 a, Vec2 b, Vec2 c, Vec2 p) {
  // get vectors between verts, as well as vec to point
  Vec2 ac = c - a;
  Vec2 ab = b - a;
  Vec2 ap = p - a;
  Vec2 pc = c - p;
  Vec2 pb = b - p;

  f32 area = (ac.x * ab.y - ac.y * ab.x);

  f32 alpha = (pc.x * pb.y - pc.y * pb.x) / area;
  f32 beta = (ac.x * ap.y - ac.y * ap.x) / area;
  f32 gamma = 1.0 - alpha - beta;
  return {alpha, beta, gamma};
}
