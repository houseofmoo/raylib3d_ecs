#pragma once

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
                PRINT("Tried to add unknown MoveIntentType component AddMovementType()");
                break;
            }
        }
    }
}