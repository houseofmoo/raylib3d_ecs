#include "systems/collisions/collision_handlers.h"

#include "data/loot.h"
#include "data/player/player.h"
#include "storage/collision_pairs.h"
#include "systems/collisions/entity_collision_system.h"
#include "components/components.h"
#include "spawners/events/loot_received_event.h"
#include "utils/rl_utils.h"
#include "utils/profiler.h"

namespace sys::col {
    void DamageOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("DamageOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (sys::col::collision_cache.previous.contains(col)) continue;

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
            if (auto* pen = world.TryGetComponent<cmpt::DestroyOnContact>(col.entity_a)) {
                pen->penetration -= 1;
                if (pen->penetration <= 0) {
                    world.AddComponent<tag::Destroy>(col.entity_a);
                }
            }

            // if B has a penetration value decrement that on collision
            if (auto* pen = world.TryGetComponent<cmpt::DestroyOnContact>(col.entity_b)) {
                pen->penetration -= 1;
                if (pen->penetration <= 0) {
                    world.AddComponent<tag::Destroy>(col.entity_b);
                }
            }
        }
    }

    void DestroyOnCollision(Storage::Registry& world) {
        PROFILE_SCOPE("DestroyOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // only terrain destroys cmpt::DestroyOnContact elements
            if (auto* a = world.TryGetComponent<tag::Terrain>(col.entity_a)) {
                if (auto* b = world.TryGetComponent<cmpt::DestroyOnContact>(col.entity_b)) {
                    world.AddComponent<tag::Destroy>(col.entity_b);
                }
            }

            if (auto* b = world.TryGetComponent<tag::Terrain>(col.entity_b)) {
                if (auto* a = world.TryGetComponent<cmpt::DestroyOnContact>(col.entity_a)) {
                    world.AddComponent<tag::Destroy>(col.entity_a);
                }
            }
        }
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
            if (auto* a = world.TryGetComponent<tag::Player>(col.entity_a)) {
                if (auto* b = world.TryGetComponent<cmpt::Loot>(col.entity_b)) {
                    world.AddComponent<tag::Destroy>(col.entity_b);
                    switch (b->kind) {
                        case data::loot::LootKind::Exp: {
                            data::player::g_player.exp += 1;
                            break;
                        }
                        case data::loot::LootKind::Money: {
                            data::player::g_player.money += 1;
                            break;
                        }
                        case data::loot::LootKind::Powerup:
                        case data::loot::LootKind::Weapon: {
                            spwn::evt::LootPickedupEvent(world, col.entity_b, b->kind);
                            break;
                        }
                        default: {
                            PRINT("UNKNOWN LOOT KIND ON PICKUP COLLISION");
                        }
                    }
                }
            }

            if (auto* b = world.TryGetComponent<tag::Player>(col.entity_b)) {
                if (auto* a = world.TryGetComponent<cmpt::Loot>(col.entity_a)) {
                    world.AddComponent<tag::Destroy>(col.entity_a);
                    switch (a->kind) {
                        case data::loot::LootKind::Exp: {
                            data::player::g_player.exp += 1;
                            break;
                        }
                        case data::loot::LootKind::Money: {
                            data::player::g_player.money += 1;
                            break;
                        }
                        case data::loot::LootKind::Powerup:
                        case data::loot::LootKind::Weapon: {
                            spwn::evt::LootPickedupEvent(world, col.entity_a, a->kind);
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
            // if entities collide with terrain, move them out of terrain
            if (auto* a = world.TryGetComponent<tag::Terrain>(col.entity_a)) {
                auto& atrans = world.GetComponent<cmpt::Transform>(col.entity_a);
                auto& acol = world.GetComponent<cmpt::Collider>(col.entity_a);
                
                auto& btrans = world.GetComponent<cmpt::Transform>(col.entity_b);
                auto& bcol = world.GetComponent<cmpt::Collider>(col.entity_b);

                auto direction = utils::PushbackDirection(
                    utils::GetBoundingBox(btrans, bcol),
                    utils::GetBoundingBox(atrans, acol)
                );
                btrans.position = Vector3Add(btrans.position, Vector3{ direction.x, 0.0f, direction.z });
            }

            if (auto* b = world.TryGetComponent<tag::Terrain>(col.entity_b)) {
                auto& btrans = world.GetComponent<cmpt::Transform>(col.entity_b);
                auto& bcol = world.GetComponent<cmpt::Collider>(col.entity_b);

                auto& atrans = world.GetComponent<cmpt::Transform>(col.entity_a);
                auto& acol = world.GetComponent<cmpt::Collider>(col.entity_a);

                auto direction = utils::PushbackDirection(
                    utils::GetBoundingBox(atrans, acol),
                    utils::GetBoundingBox(btrans, bcol)
                );
                atrans.position = Vector3Add(atrans.position, Vector3{ direction.x, 0.0f, direction.z });
            }
        }
    }
}