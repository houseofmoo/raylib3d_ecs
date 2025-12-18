#pragma once

#include "raylib.h"

namespace cmpt {
    struct Health {
        int amount;
    };

    struct Speed {
        float speed;
        float speed_multiplier;
        float dash_multiplier;
    };

    struct DamageDealer {
        int amount;
        int penetration;
    };

    struct DamageReceiver {
        int total;
    };
}