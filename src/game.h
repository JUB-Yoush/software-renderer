#pragma once
#include "ecs/world.h"

// TODO make player struct bag of pointers to values in ECS
struct Player {
};

struct PlayerRef {
    JTransform *transform;
    JMesh *mesh;
};

struct Game {
    enum RenderMode {
        WIREFRAME,
        WIREFRAME_CULLED,
        FLAT,
        SHADED,
    };

    enum ControlMode {
        CAMERA,
        PLAYER
    };

    u8 render_mode = 4;
    ControlMode control_mode = CAMERA;
    World world;
    JCamera *camera{};
    Light *light;
    PlayerRef playerref;
};
