#include "systems/collisions/collision_handlers.h"

#include "components/components.h"
#include "systems/collisions/entity_collision_system.h"
#include "utils/debug.h"

namespace sys::col {
    void DestroyOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("DestroyOnCollision()");
         for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (!sys::col::collision_cache.IsEnter(col)) continue;

            // if A is a projectile
            if (auto* proj = world.TryGetComponent<cmpt::Projectile>(col.entity_a)) {
                proj->penetration -= 1;
                if (proj->penetration <= 0) {
                    world.AddComponent<tag::Destroy>(col.entity_a);
                }
            }

            // if B is a projectile
            if (auto* proj = world.TryGetComponent<cmpt::Projectile>(col.entity_b)) {
                proj->penetration -= 1;
                if (proj->penetration <= 0) {
                    world.AddComponent<tag::Destroy>(col.entity_b);
                }
            }
        }
    }
}