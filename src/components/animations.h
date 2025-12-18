#pragma once
#include "raylib.h"

namespace cmpt {
    struct SpawnAnimation {
        Vector3 start_position;
        Vector3 end_position;
    };

    struct DeathAnimation {
        float duration;
    };

    struct DamageFlash {
        float duration;
    };
}