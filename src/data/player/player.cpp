#include "data/player/player.h"

namespace data::player {
    Player g_player = {
        .level = 1,
        .exp = 0,
        .exp_to_next_level = 10,
        .enemies_defeated = 0,
        .damage_multiplier = 1.0f,
        .attack_speed_multiplier = 1.0f,
        .move_speed_multiplier = 1.0f,
        .pickup_range_multiplier = 1.0f
    };
}