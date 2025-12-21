#pragma once

#include "storage/registry.h"
#include "data/entity.h"
#include "data/loot.h"
#include "components/components.h"
#include "utils/debug.h"

namespace spwn::evt {
    inline void LootPickedupEvent(
            Storage::Registry& world, 
            Entity id, 
            data::loot::LootKind kind,
            int amount) {
            Entity event = world.CreateEntity();

            switch (kind) {
                case data::loot::LootKind::Exp: {
                    world.AddComponent<cmpt::LootEvent>(event, cmpt::LootEvent{ .id = id, .kind = kind });
                    world.AddComponent<cmpt::ExpLoot>(event, cmpt::ExpLoot { amount });
                    break;
                }
                case data::loot::LootKind::Money: {
                    world.AddComponent<cmpt::LootEvent>(event, cmpt::LootEvent{ .id = id, .kind = kind });
                    world.AddComponent<cmpt::MoneyLoot>(event, cmpt::MoneyLoot { amount });
                    break;
                }
                default: {
                    PRINT("LootPickedupEvent() got wrong lootkind (expected exp or money, got: ", (int)kind, ")");
                }
            }
        }


    inline void LootPickedupEvent(
            Storage::Registry& world, 
            Entity id, 
            data::loot::LootKind kind,
            data::loot::PowerupKind pukind) noexcept {
        Entity event = world.CreateEntity();
        world.AddComponent<cmpt::LootEvent>(event, cmpt::LootEvent{.id = id, .kind = kind});
        world.AddComponent<cmpt::PowerupLoot>(event, cmpt::PowerupLoot{ .kind = pukind });
    }

    inline void LootPickedupEvent(
            Storage::Registry& world, 
            Entity id, 
            data::loot::LootKind kind,
            data::loot::WeaponKind wkind) noexcept {
        Entity event = world.CreateEntity();
        world.AddComponent<cmpt::LootEvent>(event, cmpt::LootEvent{.id = id, .kind = kind});
        world.AddComponent<cmpt::WeaponLoot>(event, cmpt::WeaponLoot{ .kind = wkind });
    }
}