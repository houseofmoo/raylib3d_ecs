#include "systems/collisions/collision_handlers.h"

#include "components/components.h"
#include "spawners/system/events/loot_pickedup_event.h"
#include "systems/collisions/entity_collision_system.h"

namespace sys::col {
    void HandleLootPickup(strg::Registry& world, Entity player, Entity loot) {
        auto* lootkind = world.TryGetComponent<cmpt::Loot>(loot);
        if (lootkind == nullptr) return;
        world.AddComponent<tag::Destroy>(loot);

        switch (lootkind->kind) {
            case data::loot::LootKind::Exp: {
                auto& exp = world.GetComponent<cmpt::ExpLoot>(loot);
                spwn::evt::LootPickedupEvent(world, player, lootkind->kind, exp.amount);
                break;
            }
            case data::loot::LootKind::Money: {
                auto& money = world.GetComponent<cmpt::MoneyLoot>(loot);
                spwn::evt::LootPickedupEvent(world, player, lootkind->kind, money.amount);
                break;
            }
            case data::loot::LootKind::Powerup: {
                auto& pukind = world.GetComponent<cmpt::PowerupLoot>(loot);
                spwn::evt::LootPickedupEvent(world, player, lootkind->kind, pukind.kind);
                break;
            }
            case data::loot::LootKind::Weapon: {
                auto& wepkind = world.GetComponent<cmpt::WeaponLoot>(loot);
                spwn::evt::LootPickedupEvent(world, player, lootkind->kind, wepkind.kind);
                break;
            }
            case data::loot::LootKind::WeaponCrate: {
                spwn::evt::WeaponCratePickedupEvent(world, player);
                break;
            }
            default: {
                PRINT("Unknown loot type found in PickupOnCollision() a->b");
            }
        }
    }

    void PickupOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("PickupOnCollision()");
       for (auto& col : sys::col::collision_cache.current) {

            // only players can pick up loot
            if (world.HasComponent<cmpt::Player>(col.entity_a) &&
                world.HasComponent<cmpt::Loot>(col.entity_b)) {
                HandleLootPickup(world, col.entity_a, col.entity_b);
            }

            if (world.HasComponent<cmpt::Player>(col.entity_b) &&
                world.HasComponent<cmpt::Loot>(col.entity_a)) {
                HandleLootPickup(world, col.entity_b, col.entity_a);
            }
        }
    }
}