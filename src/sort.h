#pragma once
#include "jvectors.h"
#include <algorithm>
using namespace std;

void SortPoints(Vec3 &p1, Vec3 &p2, Vec3 &p3) {
  // sort y values in decending order p3 > p2 > p1
  if (p1.y > p2.y) {
    swap(p1, p2);
  }

  if (p2.y > p3.y) {
    swap(p2, p3);
  }

  if (p1.y > p2.y) {
    swap(p1, p2);
  }
}

void SortPointsAndUVs(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec2 &uv1, Vec2 &uv2,
                      Vec2 &uv3) {

  if (p1.y > p2.y) {
    swap(p1, p2);
    swap(uv1, uv2);
  }

  if (p2.y > p3.y) {
    swap(p2, p3);
    swap(uv2, uv3);
  }

  if (p1.y > p2.y) {
    swap(p1, p2);
    swap(uv1, uv2);
  }
}