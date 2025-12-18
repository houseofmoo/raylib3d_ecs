#pragma once

#include "raylib.h"
#include "data/entity.h"
#include "data/loot.h"

namespace cmpt {
    struct WeaponStats {
        Entity parent;
        data::loot::WeaponKind kind;
        float cooldown;
        float countdown;
        float projectile_speed;
        int damage;
    };

    struct Spread {
        int pellet_count;
    };
}