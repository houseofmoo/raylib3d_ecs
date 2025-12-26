#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::proj {
    struct ExplosionConfig {
        Vector3 position;
        Vector3 start_size;
        Vector3 end_size;
        int damage;
        float duration;
        float knockback_scale;
        float knockback_duration;
    };

    void Explosion(strg::Registry& world, const ExplosionConfig config);
}