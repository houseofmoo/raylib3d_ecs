#pragma once

#include "raylib.h"
#include "storage/registry.h"
#include "components/components.h"

namespace spwn::proj {
    struct GrenadeConfig {
        // grenade info
        int damage;
        int penetration;
        float knockback_scale;
        float knockback_duration;
        float arch_duration;
        float arch_max_height;

        // explosion info
        int explosion_damage;
        Vector3 explosion_start_size;
        Vector3 explosion_end_size;
        float explosion_duration;
        float explosion_knockback_scale;
        float explosion_knockback_duration;

        Layer layer;
        Layer mask;
    };

    void Grenade(
            strg::Registry& world, 
            const Vector3 start_pos,
            const Vector3 end_pos, 
            const GrenadeConfig& config
    );
}