#pragma once

#include "storage/registry.h"
#include "data/entity.h"
#include "components/components.h"
#include "utils/debug.h"

namespace cmpt {
    // attaches collider to entity after ensure collider is at least a minimum size
    inline void AttachColliderComponent(Storage::Registry& world, const Entity id, cmpt::Collider col) noexcept {
        col.size = data::size::MinColldierSize(col.size);
        world.AddComponent<cmpt::Collider>(id, col);
    }

    // attach movement system component for AI based on move type
    inline void AttachMovementComponent(Storage::Registry& world, const Entity id, const cmpt::MoveIntentType move_type) {
        switch (move_type) {
            case cmpt::MoveIntentType::Melee: {
                // nothing
                break;
            }

            case cmpt::MoveIntentType::Ranged: {
                world.AddComponent<cmpt::RangedMovement>(
                    id,
                    cmpt::RangedMovement{ 5.0f, 10.0f }
                );
                break;
            }

            case cmpt::MoveIntentType::Lazy: {
                world.AddComponent<cmpt::LazyMovement>(
                    id,
                    cmpt::LazyMovement{ 0.0f }
                );
                break;
            }

            case cmpt::MoveIntentType::Random: {
                world.AddComponent<cmpt::RandomMovement>(
                    id,
                    cmpt::RandomMovement{ 0.0f }
                );
                break;
            }

            default: {
                PRINT("Tried to add unknown MoveIntentType component AddMovementType()");
                break;
            }
        }
    }
}