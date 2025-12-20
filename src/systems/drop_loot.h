#pragma once

#include "raylib.h"
#include "raymath.h"

#include "storage/registry.h"
#include "components/components.h"
#include "spawners/loot/loot.h"
#include "data/player/player.h"
#include "utils/debug.h"

namespace sys::loot {
    inline void LootDrop(Storage::Registry& world) {
        PROFILE_SCOPE("LootDrop()");
        for (auto enemy : world.View<tag::DropsLoot, 
                                        tag::Destroy,
                                        cmpt::Transform>()) {

            auto& etrans = world.GetComponent<cmpt::Transform>(enemy);

            // all enemies that drop loot always drop exp
            spwn::loot::Exp(world, etrans.position);

            if (data::player::g_player.always_drop_loot) {
                int roll = GetRandomValue(0, (int)data::loot::PowerupKind::Last) - 1;
                spwn::loot::Powerup(world, etrans.position, (data::loot::PowerupKind)roll);
                
                roll = GetRandomValue(0, (int)data::loot::WeaponKind::Last - 1);
                spwn::loot::Weapon(world, etrans.position, (data::loot::WeaponKind)roll);
                return;
            }

            int roll = GetRandomValue(0, 99);
            if (roll < 70) continue;

            if (roll < 80) {
                // some enemies drop money
                spwn::loot::Money(world, etrans.position);
            } else if (roll >= 80 && roll < 92) {
                // less enemies drop powerups
                roll = GetRandomValue(0, (int)data::loot::PowerupKind::Last) - 1;
                spwn::loot::Powerup(world, etrans.position, (data::loot::PowerupKind)roll);

            } else if (roll >= 92) {
                // even less enemies drop weapons
                roll = GetRandomValue(0, (int)data::loot::WeaponKind::Last - 1);
                spwn::loot::Weapon(world, etrans.position, (data::loot::WeaponKind)roll);
            }
        }
    }
}