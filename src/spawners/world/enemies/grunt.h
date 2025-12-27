#pragma once

#include "raylib.h"
#include "storage/registry.h"
#include "components/components.h"

namespace spwn::enemy {
    // TODO: config for baseline stats all enmies have
    struct EnemyArchetype {
        float speed;
        Vector3 size;
        int baseHp;
        int meleeDamage;
        float kbScale;
        float kbDuration;
        Color color;
        Model* model;
        // etc
    };

    void Grunt(strg::Registry& world, 
            const Vector3 position, 
            const cmpt::AIMoveMode move_mode, 
            const int hp);
}