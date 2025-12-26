#include "data/player/player.h"

namespace data {
    constinit Player g_player = {
        .id = 0,
        .level = 1,
        .money = 0,
        .exp = 0,
        .exp_to_next_level = 10,

        // stat multipliers
        .damage_multiplier = 1.0f,
        .attack_speed_multiplier = 1.0f,
        .move_speed_multiplier = 1.0f,
        .dash_range_multiplier = 1.0f,
        .pickup_range_multiplier = 1.0f,

        // info stats
        .enemies_defeated = 0,
    };
}