#include "systems/collisions/collision_handlers.h"

#include "raymath.h"
#include "systems/collisions/entity_collision_system.h"
#include "components/components.h"
#include "utils/position.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::col {
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
                    atrans.position + (direction * 0.5f),
                    utils::GetEntityHeight(atrans.position, acol.size)
                );

                btrans.position = utils::ValidateMovePosition(
                    btrans.position,
                    btrans.position + (direction * -0.5f),
                    utils::GetEntityHeight(btrans.position, bcol.size)
                );
            }
        }
    }
}