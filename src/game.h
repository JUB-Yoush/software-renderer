#pragma once
#include "ecs/world.h"

// TODO make player struct bag of pointers to values in ECS
struct Player {
};

typedef Vec3 Velocity;

struct Floor {
};

struct PlayerRef {
    JTransform *transform;
    JMesh *mesh;
    Velocity *velocity;
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
    ControlMode control_mode = PLAYER;
    World world;
    JCamera *camera{};
    Light *light;
    PlayerRef playerref;
    Vec2 input_dir{};
    EntityId player_id;
};
