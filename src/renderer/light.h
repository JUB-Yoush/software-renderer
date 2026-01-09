#pragma once

#include "game.h"
#include "../math/vectors.h"

struct Game; // guh?

struct Light {
    Vec3 direction;
    f32 strength;

    static void make_light(Game &game, Vec3 direction, f32 strength);
};

// Light make_light(Vec3 direction, f32 strength) {
//     return {
//         direction.normalized(),
//         strength,
//     };
// }


