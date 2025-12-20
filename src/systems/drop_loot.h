#pragma once

#include "raylib.h"
#include "raymath.h"

#include "storage/registry.h"
#include "components/components.h"
#include "spawners/loot/loot.h"
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

            int roll = GetRandomValue(0, 99);
            if (roll < 70) continue;

            if (roll < 85) {
                // some enemies drop money
                spwn::loot::Money(world, etrans.position);
            } else if (roll >= 85 && roll < 95) {
                // less enemies drop powerups
                spwn::loot::Powerup(world, etrans.position, data::loot::PowerupKind::Damage);
            } else if (roll >= 95) {
                // even less enemies drop weapons
                spwn::loot::Weapon(world, etrans.position, data::loot::WeaponKind::Rifle);
            }
        }
    }
}