#pragma once
#include "ecs/World.h"

enum RenderMode {
    WIREFRAME,
    WIREFRAME_CULLED,
    FLAT,
    SHADED,
};

struct Game {
    u8 render_mode;
    ZBuffer zbuffer;
    World world;
    JCamera *camera{};
    Light *light;
};
