#pragma once
#include <cassert>
#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"
#include "data/entity.h"
#include "components/components.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace cmpt {
    // attaches collider to entity after ensuring collider is at least a minimum size
    inline Vector3 MinLootColliderSize(Vector3 col_size) noexcept {
        if (utils::Vector3Area(col_size) < utils::Vector3Area(data::cnst::MIN_LOOT_COLLIDER)) {
            return data::cnst::MIN_LOOT_COLLIDER;
        }
        return col_size;
    }

    inline void AttachPlayerInvulnerability(strg::Registry& world, const Entity id, const float duration) {
        auto* collider = world.TryGetComponent<cmpt::Collider>(id);
        assert(collider != nullptr && "Colldier missing, AttachPlayerInvulnerability()");

        if (world.HasComponent<cmpt::Invulnerable>(id)) {
            auto& invul = world.GetComponent<cmpt::Invulnerable>(id);
            invul.countdown = invul.countdown > duration ? invul.countdown : duration;
        } else {
            world.AddComponent<cmpt::Invulnerable>(
                id,
                cmpt::Invulnerable{
                    .mask = collider->mask,
                    .countdown = duration
                }
            );

            // do not interact with enemy layer while invul
            collider->mask &= ~data::layer::ENEMY;
            collider->mask &= ~data::layer::ENEMY_PROJECTILE;
        }
    }

    // attach movement system component for AI based on move type
    inline void AttachAIMovementComponent(strg::Registry& world, const Entity id, const cmpt::AIMoveMode move_type) {
        switch (move_type) {
            case cmpt::AIMoveMode::Melee: {
                world.AddComponent<cmpt::MeleeMovement>(
                    id,
                    cmpt::MeleeMovement{ Vector3Zero(), 0.0f }
                );
                break;
            }

            case cmpt::AIMoveMode::Ranged: {
                world.AddComponent<cmpt::RangedMovement>(
                    id,
                    cmpt::RangedMovement{ 5.0f, 10.0f }
                );
                break;
            }

            case cmpt::AIMoveMode::Lazy: {
                world.AddComponent<cmpt::LazyMovement>(
                    id,
                    cmpt::LazyMovement{ 0.0f }
                );
                break;
            }

            case cmpt::AIMoveMode::Random: {
                world.AddComponent<cmpt::RandomMovement>(
                    id,
                    cmpt::RandomMovement{ 0.0f }
                );
                break;
            }

            case cmpt::AIMoveMode::None: {
                break;
            }

            default: {
                assert(false && "Tried to add unknown MoveIntentType component AddMovementType()");
                break;
            }
        }
    }
}