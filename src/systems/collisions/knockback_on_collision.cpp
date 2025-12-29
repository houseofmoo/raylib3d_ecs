#include "systems/collisions/collision_handlers.h"

#include "components/components.h"
#include "systems/collisions/entity_collision_system.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::col {
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
                        .direction = 
                            utils::DirectionFlattenThenNormalize(atrans.position, btrans.position) *
                            a->scale,
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
                        .direction = 
                            utils::DirectionFlattenThenNormalize(btrans.position, atrans.position) *
                            b->scale,
                        .countdown = b->duration 
                    }
                );
            }
        }
    }
}