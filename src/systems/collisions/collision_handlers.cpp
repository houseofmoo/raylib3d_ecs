#include "systems/collisions/collision_handlers.h"

#include "raylib.h"
#include "raymath.h"

#include "data/loot.h"
#include "data/player/player.h"
#include "data/game/game.h"
#include "storage/collision_pairs.h"
#include "systems/collisions/entity_collision_system.h"
#include "components/components.h"
#include "spawners/system/events/loot_received_event.h"
#include "spawners/system/events/notification.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys::col {
    void DamageOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("DamageOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (sys::col::collision_cache.previous.contains(col)) continue;

            // if a and b are enemies, they cannot damage eachother
            if (world.HasComponent<tag::Enemy>(col.entity_a) && world.HasComponent<tag::Enemy>(col.entity_b)) {
                continue;
            }

            // if A deals damage and B receives damage
            if (auto* dmg = world.TryGetComponent<cmpt::DamageDealer>(col.entity_a)) {
                if (auto* rcv = world.TryGetComponent<cmpt::DamageReceiver>(col.entity_b)) {
                    rcv->total += dmg->amount;
                }
            }

            // if B deals damage and A receives damage
            if (auto* dmg = world.TryGetComponent<cmpt::DamageDealer>(col.entity_b)) {
                if (auto* rcv = world.TryGetComponent<cmpt::DamageReceiver>(col.entity_a)) {
                    rcv->total += dmg->amount;
                }
            }

            // if A has a penetration value decrement that on collision
            if (auto* pen = world.TryGetComponent<cmpt::Penetration>(col.entity_a)) {
                pen->amount -= 1;
                if (pen->amount <= 0) {
                    world.AddComponent<tag::Destroy>(col.entity_a);
                }
            }

            // if B has a penetration value decrement that on collision
            if (auto* pen = world.TryGetComponent<cmpt::Penetration>(col.entity_b)) {
                pen->amount -= 1;
                if (pen->amount <= 0) {
                    world.AddComponent<tag::Destroy>(col.entity_b);
                }
            }
        }
    }

    void DestroyOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("DestroyOnCollision()");
        // // TODO: since only terrain causes destruction on collision
        // // without thought, we just do this in the movement system
        // for (auto& col : sys::col::collision_cache.current) {
        //     // only terrain destroys cmpt::DestroyOnContact elements
        //     if (auto* a = world.TryGetComponent<tag::Terrain>(col.entity_a)) {
        //         if (auto* b = world.TryGetComponent<cmpt::DestroyOnContact>(col.entity_b)) {
        //             world.AddComponent<tag::Destroy>(col.entity_b);
        //         }
        //     }

        //     if (auto* b = world.TryGetComponent<tag::Terrain>(col.entity_b)) {
        //         if (auto* a = world.TryGetComponent<cmpt::DestroyOnContact>(col.entity_a)) {
        //             world.AddComponent<tag::Destroy>(col.entity_a);
        //         }
        //     }
        // }
    }

    void KnockbackOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("KnockbackOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // players knocked back when collided with enemy
            if (auto* a = world.TryGetComponent<tag::Player>(col.entity_a)) {
                if (auto* b = world.TryGetComponent<tag::Enemy>(col.entity_b)) {
                    auto& ptrans = world.GetComponent<cmpt::Transform>(col.entity_a);
                    auto& etrans = world.GetComponent<cmpt::Transform>(col.entity_b);
                    auto dir = Vector3Subtract(etrans.position, ptrans.position);
                    dir = Vector3Scale(Vector3Normalize({dir.x, 0.0f, dir.z}), -20.0f);
                    world.AddComponent<cmpt::Knockback>(col.entity_a, cmpt::Knockback{ dir, 0.1f });
                }
            }

            if (auto* a = world.TryGetComponent<tag::Enemy>(col.entity_a)) {
                if (auto* b = world.TryGetComponent<tag::Player>(col.entity_b)) {
                    auto& ptrans = world.GetComponent<cmpt::Transform>(col.entity_b);
                    auto& etrans = world.GetComponent<cmpt::Transform>(col.entity_a);
                    auto dir = Vector3Subtract(etrans.position, ptrans.position);
                    dir = Vector3Scale(Vector3Normalize({dir.x, 0.0f, dir.z}), -20.0f);
                    world.AddComponent<cmpt::Knockback>(col.entity_b, cmpt::Knockback{ dir, 0.1f });
                }
            }
        }
    }

    void PickupOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("PickupOnCollision()");
       for (auto& col : sys::col::collision_cache.current) {

            // only players can pick up loot
            if (auto* player = world.TryGetComponent<tag::Player>(col.entity_a)) {
                if (auto* loot = world.TryGetComponent<cmpt::Loot>(col.entity_b)) {
                    world.AddComponent<tag::Destroy>(col.entity_b);
                    switch (loot->kind) {
                        case data::loot::LootKind::Exp: {
                            auto& exp = world.GetComponent<cmpt::ExpLoot>(col.entity_b);
                            spwn::evt::LootPickedupEvent(world, col.entity_a, loot->kind, exp.amount);
                            break;
                        }
                        case data::loot::LootKind::Money: {
                            auto& money = world.GetComponent<cmpt::MoneyLoot>(col.entity_b);
                            spwn::evt::LootPickedupEvent(world, col.entity_a, loot->kind, money.amount);
                            break;
                        }
                        case data::loot::LootKind::Powerup: {
                            auto& pukind = world.GetComponent<cmpt::PowerupLoot>(col.entity_b);
                            spwn::evt::LootPickedupEvent(world, col.entity_a, loot->kind, pukind.kind);
                            break;
                        }
                        case data::loot::LootKind::Weapon: {
                            auto& wepkind = world.GetComponent<cmpt::WeaponLoot>(col.entity_b);
                            spwn::evt::LootPickedupEvent(world, col.entity_a, loot->kind, wepkind.kind);
                            break;
                        }
                        default: {
                            PRINT("Unknown loot type found in PickupOnCollision()");
                        }
                    }
                }
            }

            if (auto* player = world.TryGetComponent<tag::Player>(col.entity_b)) {
                if (auto* loot = world.TryGetComponent<cmpt::Loot>(col.entity_a)) {
                    world.AddComponent<tag::Destroy>(col.entity_a);
                    switch (loot->kind) {
                        case data::loot::LootKind::Exp: {
                            auto& exp = world.GetComponent<cmpt::ExpLoot>(col.entity_a);
                            spwn::evt::LootPickedupEvent(world, col.entity_b, loot->kind, exp.amount);
                            break;
                        }
                        case data::loot::LootKind::Money: {
                            auto& money = world.GetComponent<cmpt::MoneyLoot>(col.entity_a);
                            spwn::evt::LootPickedupEvent(world, col.entity_b, loot->kind, money.amount);
                            break;
                        }
                        case data::loot::LootKind::Powerup: {
                            auto& pukind = world.GetComponent<cmpt::PowerupLoot>(col.entity_a);
                            spwn::evt::LootPickedupEvent(world, col.entity_b, loot->kind, pukind.kind);
                            break;
                        }
                        case data::loot::LootKind::Weapon: {
                            auto& wepkind = world.GetComponent<cmpt::WeaponLoot>(col.entity_a);
                            spwn::evt::LootPickedupEvent(world, col.entity_b, loot->kind, wepkind.kind);
                            break;
                        }
                        default: {
                            PRINT("UNKNOWN LOOT KIND ON PICKUP COLLISION");
                        }
                    }
                }
            }
        }
    }

    void TriggerOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("TriggerOnCollision()");
    }

    void RepositionOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("RepositionOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // enemies cannot overlap
            if (auto* a = world.TryGetComponent<tag::Enemy>(col.entity_a)) {
                if (auto* b = world.TryGetComponent<tag::Enemy>(col.entity_b)) {
                    // push enenmies out of each other
                    auto& btrans = world.GetComponent<cmpt::Transform>(col.entity_b);
                    auto& bcol = world.GetComponent<cmpt::Collider>(col.entity_b);

                    auto& atrans = world.GetComponent<cmpt::Transform>(col.entity_a);
                    auto& acol = world.GetComponent<cmpt::Collider>(col.entity_a);
                    auto direction = utils::PushbackMTV_XZ(
                        utils::GetBoundingBox(atrans, acol),
                        utils::GetBoundingBox(btrans, bcol)
                    );

                    // apply velocty away from each other instead of modifying position directly
                    if (direction.x != 0.0f || direction.z != 0) {
                        utils::MoveAndSlideTerrain(atrans.position, Vector3Scale(direction,  0.5f));
                        utils::MoveAndSlideTerrain(btrans.position, Vector3Scale(direction, -0.5f));
                    }
                }
            }
        }
    }
}