#pragma once
#include "ecs/world.h"
#include "renderer/mesh.h"
#include "renderer/camera.h"
#include "renderer/light.h"

struct JCamera; // guh?
struct Light;

// TODO make player struct bag of pointers to values in ECS
typedef Vec3 Velocity;

struct Player {
};

struct Mob {
};


struct Floor {
};

struct PlayerRef {
    JTransform *transform;
    JMesh *mesh;
    Velocity *velocity;
};

struct Game; // guh?

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
    JCamera *camera;
    Light *light;
    PlayerRef playerref;
    Vec2 input_dir{};
    EntityId player_id;
};


void make_player(Game &game) {
    EntityId player_id = game.world.new_entity();
    game.player_id = player_id;
    auto *player = game.world.assign<Player>(player_id);
    auto *pmesh = game.world.assign<JMesh>(player_id);
    auto *ptransform = game.world.assign<JTransform>(player_id);
    auto *player_aabb = game.world.assign<AABB>(player_id);
    auto *player_vel = game.world.assign<Velocity>(player_id);

    // TODO why is everything upside down
    ptransform->translation.y += -3;

    *pmesh = make_rectangle_mesh(1, 1, 1);
    *player_aabb = AABB::make(1, 1, 1);

    pmesh->color = RED;
    game.playerref.transform = ptransform;
    game.playerref.mesh = pmesh;
    game.playerref.velocity = player_vel;
}

void make_floor(Game &game) {
    EntityId floor = game.world.new_entity();
    auto *floor_mesh = game.world.assign<JMesh>(floor);
    game.world.assign<Floor>(floor);
    auto *floor_trans = game.world.assign<JTransform>(floor);
    auto *floor_aabb = game.world.assign<AABB>(floor);
    *floor_aabb = AABB::make(10, .5, 10);
    *floor_mesh = make_aabb_mesh(*floor_aabb);
}


