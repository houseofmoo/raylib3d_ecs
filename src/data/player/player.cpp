#include "data/player/player.h"

namespace data::player {
    Player g_player = {
        .id = 0,
        .level = 1,
        .money = 0,
        .exp = 0,
        .exp_to_next_level = 10,
        
        .enemies_defeated = 0,

        .damage_multiplier = 1.0f,
        .attack_speed_multiplier = 1.0f,
        .move_speed_multiplier = 1.0f,
        .pickup_range_multiplier = 1.0f,

        // cheats
        .god_mode = false,
        .always_drop_loot = false,
        .profiler_enabled = false,
    };
}