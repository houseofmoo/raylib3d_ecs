#pragma once

#include "raylib.h"

namespace cmpt {
    struct Invulnerable {
        float countdown;
    };

    struct Dash {
        float multiplier;
        float countdown;
    };


    struct DashExhausted { // not allowed to dash until some time
        float countdown;
    };

    struct Knockback {
        Vector3 direction;
        float countdown;
    };
}