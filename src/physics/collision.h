#pragma once
#include "../jlib.h"
#include "../math/jvectors.h"

struct AABB {
    f32 min_x;
    f32 max_x;
    f32 min_y;
    f32 max_y;
    f32 min_z;
    f32 max_z;

    static bool intersects(const AABB a, const AABB b) {
        return (
            a.min_x <= b.max_x &&
            a.max_x >= b.min_x &&
            a.min_y <= b.max_y &&
            a.max_y >= b.min_y &&
            a.min_z <= b.max_z &&
            a.max_z >= b.min_z
        );
    }

    static bool within_point(const AABB box, const Vec3 point) {
        return point.x >= box.min_x && point.x <= box.max_x &&
               point.y >= box.min_y && point.y <= box.max_y &&
               point.z >= box.min_z && point.z <= box.max_z;
    }
};
