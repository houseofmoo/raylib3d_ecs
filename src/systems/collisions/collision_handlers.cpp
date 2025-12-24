#include "systems/collisions/collision_handlers.h"

#include "raylib.h"
#include "raymath.h"

#include "data/loot.h"
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
    void DamageOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("DamageOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (sys::col::collision_cache.previous.contains(col)) continue;

            // if a and b are enemies, they cannot damage eachother
            if (world.HasComponent<tag::Enemy>(col.entity_a) && 
                world.HasComponent<tag::Enemy>(col.entity_b)) {
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
        }
    }

    void DestroyOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("DestroyOnCollision()");
         for (auto& col : sys::col::collision_cache.current) {
            // if A is a projectile
            if (world.HasComponent<tag::Projectile>(col.entity_a)) {
                // penetration -> destroy once penetration value is 0
                if (auto* pen = world.TryGetComponent<cmpt::Penetration>(col.entity_a)) {
                    pen->amount -= 1;
                    if (pen->amount <= 0) {
                        world.AddComponent<tag::Destroy>(col.entity_a);
                    }
                }

                // explosive -> destroy and spawn damage area
                if (auto exp = world.TryGetComponent<cmpt::ExplodeOnImpact>(col.entity_a)) {
                    //world.AddComponent<tag::Destroy>(col.entity_a);
                    // TODO: spawn area damage request
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

                // explosive -> destroy and spawn damage  area
                if (auto exp = world.TryGetComponent<cmpt::ExplodeOnImpact>(col.entity_b)) {
                    //world.AddComponent<tag::Destroy>(col.entity_b);
                    // TODO: spawn area damage request
                }
            }
        }
    }

    void KnockbackOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("KnockbackOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
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
                        case data::loot::LootKind::WeaponCrate: {
                            spwn::evt::WeaponCratePickedupEvent(world, col.entity_a);
                        }
                        default: {
                            PRINT("Unknown loot type found in PickupOnCollision() a->b");
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
                        case data::loot::LootKind::WeaponCrate: {
                            spwn::evt::WeaponCratePickedupEvent(world, col.entity_b);
                        }
                        default: {
                            PRINT("Unknown loot type found in PickupOnCollision() b->a");
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
            if (direction.x != 0.0f || direction.z != 0) {
                utils::MoveAndSlideTerrain(atrans.position, Vector3Scale(direction,  0.5f));
                utils::MoveAndSlideTerrain(btrans.position, Vector3Scale(direction, -0.5f));
            }
        }
    }
}