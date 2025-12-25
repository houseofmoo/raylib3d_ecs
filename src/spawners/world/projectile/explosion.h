#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::proj {
    struct ExplosionConfig {
        Vector3 position;
        Vector3 start_size;
        Vector3 expansion_steps;
        float duration;
        int damage;
        float knockback_scale;
        float knockback_duration;
    };

    void Explosion(Storage::Registry& world, const ExplosionConfig config);
}