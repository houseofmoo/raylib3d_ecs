#include "systems/collisions/collision_handlers.h"

#include "raylib.h"
#include "raymath.h"

#include "data/player/player.h"
#include "data/game/game.h"
#include "storage/collision_pairs.h"
#include "systems/collisions/entity_collision_system.h"
#include "components/components.h"
#include "spawners/system/events/loot_pickedup_event.h"
#include "spawners/system/events/notification.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys::col {
    void DamageOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("DamageOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (!sys::col::collision_cache.IsEnter(col)) continue;

            // if a and b are enemies, they cannot damage eachother
            if (world.HasComponent<tag::Enemy>(col.entity_a) &&
                world.HasComponent<tag::Enemy>(col.entity_b)) continue;
            
            // if A deals damage and B receives damage
            auto* a_dmg = world.TryGetComponent<cmpt::DamageDealer>(col.entity_a);
            auto* b_dmg = world.TryGetComponent<cmpt::DamageDealer>(col.entity_b);
            auto* a_rcv = world.TryGetComponent<cmpt::DamageReceiver>(col.entity_a);
            auto* b_rcv = world.TryGetComponent<cmpt::DamageReceiver>(col.entity_b);

            if (a_dmg != nullptr && b_rcv != nullptr) {
                b_rcv->total += a_dmg->amount;
            }

            if (b_dmg != nullptr && a_rcv != nullptr) {
                a_rcv->total += b_dmg->amount;
            }
        }
    }

    void DestroyOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("DestroyOnCollision()");
         for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (!sys::col::collision_cache.IsEnter(col)) continue;

            // if A is a projectile
            if (world.HasComponent<tag::Projectile>(col.entity_a)) {
                // penetration -> destroy once penetration value is 0
                if (auto* pen = world.TryGetComponent<cmpt::Penetration>(col.entity_a)) {
                    pen->amount -= 1;
                    if (pen->amount <= 0) {
                        world.AddComponent<tag::Destroy>(col.entity_a);
                    }
                }
            }

            // if B is a projectile
            if (world.HasComponent<tag::Projectile>(col.entity_b)) {
                // penetration -> destroy once penetration value is 0
                if (auto* pen = world.TryGetComponent<cmpt::Penetration>(col.entity_b)) {
                    pen->amount -= 1;
                    if (pen->amount <= 0) {
                        world.AddComponent<tag::Destroy>(col.entity_b);
                    }
                }
            }
        }
    }

    void KnockbackOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("KnockbackOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (!sys::col::collision_cache.IsEnter(col)) continue;

            // if a and b are enemies, they do not knock eachother back
            if (world.HasComponent<tag::Enemy>(col.entity_a) && 
                world.HasComponent<tag::Enemy>(col.entity_b)) {
                continue;
            }

            // A applies knockback to B
            if (auto* a = world.TryGetComponent<cmpt::AppliesKnockback>(col.entity_a)) {
                auto& atrans = world.GetComponent<cmpt::Transform>(col.entity_a);
                auto& btrans = world.GetComponent<cmpt::Transform>(col.entity_b);

                world.AddComponent<cmpt::Knockback>(
                    col.entity_b, 
                    cmpt::Knockback{ 
                        .direction = Vector3Scale(
                            utils::DirectionFlattenThenNormalize(atrans.position, btrans.position),
                            a->scale
                        ), 
                        .countdown = a->duration 
                    }
                );
            }

            // B applies knockback to A
            if (auto* b = world.TryGetComponent<cmpt::AppliesKnockback>(col.entity_b)) {
                auto& atrans = world.GetComponent<cmpt::Transform>(col.entity_a);
                auto& btrans = world.GetComponent<cmpt::Transform>(col.entity_b);

                world.AddComponent<cmpt::Knockback>(
                    col.entity_a, 
                    cmpt::Knockback{ 
                        .direction = Vector3Scale(
                            utils::DirectionFlattenThenNormalize(btrans.position, atrans.position),
                            b->scale
                        ),
                        .countdown = b->duration 
                    }
                );
            }
        }
    }

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
            if (world.HasComponent<tag::Player>(col.entity_a) &&
                world.HasComponent<cmpt::Loot>(col.entity_b)) {
                HandleLootPickup(world, col.entity_a, col.entity_b);
            }

            if (world.HasComponent<tag::Player>(col.entity_b) &&
                world.HasComponent<cmpt::Loot>(col.entity_a)) {
                HandleLootPickup(world, col.entity_b, col.entity_a);
            }
        }
    }

    void TriggerOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("TriggerOnCollision()");
    }

    void RepositionOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("RepositionOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // enemies cannot overlap
            if (!world.HasComponent<tag::Enemy>(col.entity_a) ||
                !world.HasComponent<tag::Enemy>(col.entity_b)) {
                continue;
            }
            
            // push enenmies out of each other
            auto& btrans = world.GetComponent<cmpt::Transform>(col.entity_b);
            auto& bcol = world.GetComponent<cmpt::Collider>(col.entity_b);

            auto& atrans = world.GetComponent<cmpt::Transform>(col.entity_a);
            auto& acol = world.GetComponent<cmpt::Collider>(col.entity_a);
            auto direction = utils::PushbackMTV_XZ(
                utils::GetBoundingBox(atrans, acol),
                utils::GetBoundingBox(btrans, bcol)
            );

            // other than velocity system, this should be the only place that modifies transforms
            if (Vector3LengthSqr(direction) > 0.0001f) {
                // TODO: maybe set stuck=true here? so enemies colliding also results in them wandering off in a new direction
                atrans.position = utils::ValidateMovePosition(
                    atrans.position,
                    Vector3Add(atrans.position, Vector3Scale(direction, 0.5f)),
                    utils::GetEntityHeight(atrans.position, acol.size)
                );

                btrans.position = utils::ValidateMovePosition(
                    btrans.position,
                    Vector3Add(btrans.position, Vector3Scale(direction, -0.5f)),
                    utils::GetEntityHeight(btrans.position, bcol.size)
                );
            }
        }
    }
}