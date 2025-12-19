#pragma once

#include "data/entity.h"

namespace data::player {
    struct Player {
        Entity id;
        int level;
        int money;
        int exp;
        int exp_to_next_level;
        int enemies_defeated;

        float damage_multiplier;
        float attack_speed_multiplier;
        float move_speed_multiplier;
        float pickup_range_multiplier;
    };

    extern Player g_player;
}