#pragma once

#include <cinttypes>
#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"
#include "components/components.h"
#include "spawners/world/loot/loot.h"
#include "data/game/game.h"
#include "data/entity.h"
#include "utils/debug.h"

namespace sys::loot {
    inline void LootDrop(strg::Registry& world) {
        PROFILE_SCOPE("LootDrop()");
        static uint32_t enemies_since_loot_dropped = 0;
        
        if (data::g_cheats.never_drop_loot) {
            return;
        }
        
        for (auto enemy : world.View<cmpt::DropsLoot, 
                                        tag::Destroy,
                                        cmpt::Transform>()) {
            
            auto& drop = world.GetComponent<cmpt::DropsLoot>(enemy);                                   
            auto& etrans = world.GetComponent<cmpt::Transform>(enemy);

            float collider_scale = 1.0f;
            if (auto* stats = world.TryGetComponent<cmpt::Stats>(data::g_player_id)) {
                collider_scale = stats->pickup_range_modifier;
            }

            // all enemies that drop loot always drop exp
            spwn::loot::Exp(world, etrans.position, data::cnst::EXP_VALUE, collider_scale);

            if (data::g_cheats.always_drop_loot) {
                spwn::loot::Powerup(world, etrans.position, data::loot::GetRandomPowerupKind(), collider_scale);
                //spwn::loot::Weapon(world, etrans.position, data::loot::GetRandomWeaponKind());
                spwn::loot::WeaponCrate(world, etrans.position, collider_scale);
                continue;
            }

            int roll = GetRandomValue(0, 99);
            
            // if 10 enemies die and no loot has dropped, drop loot
            if (enemies_since_loot_dropped >= data::cnst::LOOT_BADLUCK_PROTECTION) {
                if (roll < 50) {
                    spwn::loot::Powerup(world, etrans.position, data::loot::GetRandomPowerupKind(), collider_scale);
                } else {
                    //spwn::loot::Weapon(world, etrans.position, data::loot::GetRandomWeaponKind());
                    spwn::loot::WeaponCrate(world, etrans.position, collider_scale);
                }
                enemies_since_loot_dropped = 0;
                continue;
            }

            // check if loot roll was success
            roll = static_cast<int>((float)roll * drop.loot_chance);
            if (roll < data::cnst::LOOT_CHANCE) {
                enemies_since_loot_dropped += 1;
                continue;
            }

            // roll for loot type
            roll = GetRandomValue(0, 99);
            if (roll < 40) {
                spwn::loot::Money(world, etrans.position, data::cnst::MONEY_VALUE, collider_scale);
                enemies_since_loot_dropped += 1; // money doesnt count (we generous like that)
            } else if (roll >= 40 && roll < 80) {
                spwn::loot::Powerup(world, etrans.position, data::loot::GetRandomPowerupKind(), collider_scale);
                enemies_since_loot_dropped = 0;
            } else if (roll >= 80) {
                //spwn::loot::Weapon(world, etrans.position, data::loot::GetRandomWeaponKind());
                spwn::loot::WeaponCrate(world, etrans.position, collider_scale);
                enemies_since_loot_dropped = 0;
            }
        }
    }
}