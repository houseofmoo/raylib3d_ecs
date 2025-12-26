#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::proj {
    struct BulletConfig {
        Vector3 position;
        Vector3 direction;
        int damage;
        int penetration;
        float knockback_scale;
        float knockback_duration;
    };

    void Bullet(strg::Registry& world, const BulletConfig config);
}