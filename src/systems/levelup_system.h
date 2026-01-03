
#pragma once

#include "storage/registry.h"
#include "state/nav_request.h"
#include "components/components.h"
#include "spawners/system/events/notification.h"
#include "utils/debug.h"

namespace sys::lvl {
      void UpgradeStats(strg::Registry& world, Entity id) {
        auto* stats = world.TryGetComponent<cmpt::Stats>(id);
        stats->damage_modifier += data::cnst::LEVEL_UP_DAMAGE_VALUE;
        stats->attack_speed_modifier += data::cnst::LEVEL_UP_ATTACK_SPEED_VALUE;
        stats->move_speed_modifier += data::cnst::LEVEL_UP_MOVE_SPEED_VALUE;
        stats->pickup_range_modifier += data::cnst::LEVEL_UP_PICKUP_RANGE_VALUE;
        stats->dash_speed_modifier += data::cnst::LEVEL_UP_DASH_DISTANCE_VALUE;
    }

    inline void LevelupSystem(strg::Registry& world, state::NavRequest& nav) {
        PROFILE_SCOPE("LevelupSystem()");
        for (auto entity : world.View<cmpt::Player, cmpt::Stats>()) {
            auto& player = world.GetComponent<cmpt::Player>(entity);

            if (player.exp < player.exp_to_level) continue;
            auto& stats = world.GetComponent<cmpt::Stats>(entity);

            while (player.exp >= player.exp_to_level) {
                player.exp  = player.exp - player.exp_to_level;
                player.level += 1;
                player.exp_to_level *= data::cnst::PLAYER_EXP_MODIFIER;
                
                stats.damage_modifier += data::cnst::LEVEL_UP_DAMAGE_VALUE;
                stats.attack_speed_modifier += data::cnst::LEVEL_UP_ATTACK_SPEED_VALUE;
                stats.move_speed_modifier += data::cnst::LEVEL_UP_MOVE_SPEED_VALUE;
                stats.pickup_range_modifier += data::cnst::LEVEL_UP_PICKUP_RANGE_VALUE;
                stats.dash_speed_modifier += data::cnst::LEVEL_UP_DASH_DISTANCE_VALUE;
                
                spwn::evt::Notification(world, data::notif::GAIN_LEVELUP);
                nav.MakeRequest(state::NavRequestKind::PlayerLevelup);
            }
        }
    }
}