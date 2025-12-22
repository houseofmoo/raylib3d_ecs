#pragma once

#include "data/entity.h"

namespace data {
    struct Player {
        Entity id;
        int level;
        int money;
        int exp;
        int exp_to_next_level;

        // stat multipliers
        float damage_multiplier;
        float attack_speed_multiplier;
        float move_speed_multiplier;
        float dash_range_multiplier;
        float pickup_range_multiplier;

        // info stats
        int enemies_defeated;

        // cheats
        bool god_mode;
        bool always_drop_loot;
        bool profiler_enabled;

        // returns new level on level up, or 0 if no level up
        int AddExp(int exp_amount) {
            exp += exp_amount;

            // did not level up
            if (exp < exp_to_next_level) {
                return 0;
            }

            // did level up, continue leveling up until exp < exp_to_next_level
            while (exp >= exp_to_next_level) {
                exp = exp - exp_to_next_level;
                level += 1;
                exp_to_next_level *= 1.15f; // 15% more exp each level
                
                damage_multiplier += 0.01f;
                attack_speed_multiplier += 0.01f;
                move_speed_multiplier += 0.01f;
                dash_range_multiplier += 0.01f;
                pickup_range_multiplier += 0.01f;
            }
            return level;
        }
    };

    extern Player g_player;
}