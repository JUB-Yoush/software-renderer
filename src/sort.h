#pragma once
#include "jvectors.h"
#include <algorithm>
using namespace std;

void SortPoints(Vec3 &p1, Vec3 &p2, Vec3 &p3) {
  if (p1.y > p2.y) {
    swap(p1.x, p2.x);
    swap(p1.y, p2.y);
    swap(p1.z, p2.z);
  }

  if (p2.y > p3.y) {
    swap(p2.x, p3.x);
    swap(p2.y, p3.y);
    swap(p2.z, p3.z);
  }

  if (p1.y > p2.y) {
    swap(p1.x, p2.x);
    swap(p1.y, p2.y);
    swap(p1.z, p2.z);
  }
}