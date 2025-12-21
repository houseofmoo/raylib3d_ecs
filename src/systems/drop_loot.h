#pragma once

#include <cinttypes>
#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"
#include "components/components.h"
#include "spawners/world/loot/loot.h"
#include "data/player/player.h"
#include "utils/debug.h"

namespace sys::loot {
    inline void LootDrop(Storage::Registry& world) {
        PROFILE_SCOPE("LootDrop()");
        static uint32_t enemies_since_loot_dropped = 0;
        for (auto enemy : world.View<cmpt::DropsLoot, 
                                        tag::Destroy,
                                        cmpt::Transform>()) {
            
            
            auto& chance = world.GetComponent<cmpt::DropsLoot>(enemy);                                   
            auto& etrans = world.GetComponent<cmpt::Transform>(enemy);

            // all enemies that drop loot always drop exp
            spwn::loot::Exp(world, etrans.position, 1);

            if (data::player::g_player.always_drop_loot) {
                int roll = GetRandomValue(0, (int)data::loot::PowerupKind::Last) - 1;
                spwn::loot::Powerup(world, etrans.position, (data::loot::PowerupKind)roll);
                
                roll = GetRandomValue(0, (int)data::loot::WeaponKind::Last - 1);
                spwn::loot::Weapon(world, etrans.position, (data::loot::WeaponKind)roll);
                continue;
            }

            // if 10 enemies die and no loot has dropped, drop loot
            int roll = GetRandomValue(0, 99);
            if (enemies_since_loot_dropped > 10) {
                if (roll < 50) {
                    roll = GetRandomValue(0, (int)data::loot::PowerupKind::Last) - 1;
                    spwn::loot::Powerup(world, etrans.position, (data::loot::PowerupKind)roll);
                } else {
                    roll = GetRandomValue(0, (int)data::loot::WeaponKind::Last - 1);
                    spwn::loot::Weapon(world, etrans.position, (data::loot::WeaponKind)roll);
                }
                enemies_since_loot_dropped = 0;
                continue;
            }

            // check if loot roll was success
            if (roll < 50) {
                // if roll unsuccessful, add roll multiplier and recheck
                roll *= chance.loot_chance;
                if (roll < 50)  {
                    enemies_since_loot_dropped += 1;
                    continue;
                }
            }

            // roll for loot type
            enemies_since_loot_dropped = 0;
            roll = GetRandomValue(0, 99);

            if (roll < 40) {
                spwn::loot::Money(world, etrans.position, 1);
            } else if (roll >= 40 && roll < 90) {
                roll = GetRandomValue(0, (int)data::loot::PowerupKind::Last - 1);
                spwn::loot::Powerup(world, etrans.position, (data::loot::PowerupKind)roll);
            } else if (roll >= 90) {
                roll = GetRandomValue(0, (int)data::loot::WeaponKind::Last - 1);
                spwn::loot::Weapon(world, etrans.position, (data::loot::WeaponKind)roll);
            }
        }
    }
}