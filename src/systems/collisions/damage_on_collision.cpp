#include "systems/collisions/collision_handlers.h"

#include "systems/collisions/entity_collision_system.h"
#include "components/components.h"
#include "utils/debug.h"

namespace sys::col {
    void DamageOnCollision(strg::Registry& world) {
        PROFILE_SCOPE("DamageOnCollision()");
        for (auto& col : sys::col::collision_cache.current) {
            // if A and B interacted last frame, ignore them this frame
            if (!sys::col::collision_cache.IsEnter(col)) continue;

            // if a and b are enemies, they cannot damage eachother
            if (world.HasComponent<tag::Enemy>(col.entity_a) &&
                world.HasComponent<tag::Enemy>(col.entity_b)) continue;
            
            // if A deals damage and B receives damage
            auto* a_dmg = world.TryGetComponent<cmpt::DamageDealer>(col.entity_a);
            auto* b_dmg = world.TryGetComponent<cmpt::DamageDealer>(col.entity_b);
            auto* a_rcv = world.TryGetComponent<cmpt::DamageReceiver>(col.entity_a);
            auto* b_rcv = world.TryGetComponent<cmpt::DamageReceiver>(col.entity_b);

            if (a_dmg != nullptr && b_rcv != nullptr) {
                b_rcv->total += a_dmg->amount;
            }

            if (b_dmg != nullptr && a_rcv != nullptr) {
                a_rcv->total += b_dmg->amount;
            }
        }
    }
}