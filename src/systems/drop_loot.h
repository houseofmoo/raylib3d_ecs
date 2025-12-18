#pragma once

#include "raylib.h"
#include "raymath.h"

#include "storage/registry.h"
#include "components/components.h"
#include "components/tags.h"
#include "spawners/loot/loot.h"

namespace sys::loot {
    inline void LootDrop(Storage::Registry& world) {
        for (auto enemy : world.View<tag::DropsLoot, 
                                        tag::Destroy,
                                        cmpt::Transform>()) {

            auto& etrans = world.GetComponent<cmpt::Transform>(enemy);

            spwn::loot::Loot(
                world,
                Vector3Add(etrans.position, {1.0f, 0.0f, 1.0f}),
                data::loot::LootKind::Exp,
                SKYBLUE
            );
            
            int roll = GetRandomValue(0, 99);
            if (roll < 70) continue;
            
            // additional drops
            if (roll >= 70 && roll < 78) {
                spwn::loot::Loot(
                    world, 
                    etrans.position, 
                    data::loot::LootKind::Damage, 
                    YELLOW
                );
            } else if (roll >= 78 && roll < 85) {
                spwn::loot::Loot(
                    world, 
                    etrans.position, 
                    data::loot::LootKind::PelletCount, 
                    ORANGE
                );
            } else if (roll >= 85 && roll < 92) {
                spwn::loot::Loot(
                    world, 
                    etrans.position, 
                    data::loot::LootKind::AttackSpeed, 
                    GREEN
                );
            } else if (roll >= 92) {
                spwn::loot::Loot(
                    world, 
                    etrans.position, 
                    data::loot::LootKind::Penetration, 
                    BEIGE
                );
            }
        }
    }
}