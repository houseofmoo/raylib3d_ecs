#pragma once

#include "storage/registry.h"
#include "spawners/events/notification.h"
#include "data/player/player.h"

namespace sys::lvl {
    inline void PlayerLevelup(Storage::Registry& world) {
        if (data::player::g_player.exp >= data::player::g_player.exp_to_next_level) {
            data::player::g_player.level += 1;
            data::player::g_player.exp_to_next_level *= 1.2f;
            data::player::g_player.exp = data::player::g_player.exp / data::player::g_player.exp_to_next_level;
            
            data::player::g_player.damage_multiplier += 0.01f;
            data::player::g_player.attack_speed_multiplier += 0.01f;
            data::player::g_player.move_speed_multiplier += 0.01f;
            data::player::g_player.pickup_range_multiplier += 0.1f;
            // player some animation/sound effect

            spwn::evt::Notification(world, std::string("LVL UP " + std::to_string(data::player::g_player.level)));
        }
    }
}