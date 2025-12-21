#pragma once

#include "data/entity.h"
#include "storage/registry.h"
#include "components/components.h"
#include "utils/debug.h"

namespace cmpt::add {
    inline void AddMovementType(
        Storage::Registry& world, 
        const Entity id, 
        const cmpt::MoveIntentType move_type) {

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