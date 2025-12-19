#include "systems/collision/entity_collision_system.h"

#include <vector>
#include "components/components.h"
#include "components/tags.h"
#include "utils/rl_utils.h"
#include "utils/profiler.h"

namespace sys::col {
    strg::CollisionEnterCache collision_cache;

    void EntityCollision(Storage::Registry& world) {
        PROFILE_SCOPE("EntityCollision()");

        // set up for next collision check
        std::swap(collision_cache.previous, collision_cache.current);
        collision_cache.current.clear();

        // get all entities into one vector
        std::vector<Entity> entities;
        entities.reserve(world.Count<cmpt::Transform, cmpt::Collider>());
        for (auto entity : world.View<cmpt::Transform, cmpt::Collider>()) {
            entities.push_back(entity);
        }

        // iterate over all entites without redoing checks (A -> B without B -> A)
        for (int i = 0; i < entities.size(); ++i) {
            Entity a = entities[i];

            auto& atrans = world.GetComponent<cmpt::Transform>(a);
            auto& acol = world.GetComponent<cmpt::Collider>(a);
            auto abox = utils::GetBoundingBox(atrans, acol);

            for (int j = i + 1; j < entities.size(); ++j) {
                Entity b = entities[j];

                // do these layer interact?
                auto& bcol = world.GetComponent<cmpt::Collider>(b);
                bool interacts = data::layer::InteractsBoth(acol.layer, acol.mask, bcol.layer, bcol.mask);
                //bool interacts = data::layer::InteractsOneWay(acol.layer, acol.mask, bcol.layer, bcol.mask);
                if (!interacts) continue;
                
                // are these colliding?
                auto& btrans = world.GetComponent<cmpt::Transform>(b);
                auto bbox = utils::GetBoundingBox(btrans, bcol);
                if (!CheckCollisionBoxes(abox, bbox)) continue;

                // new collision
                collision_cache.current.insert(strg::CollisionPair{ a, b });
            }
        }
    }


    // void EntityCollision(Storage::Registry& world) {
    //     std::swap(collision_cache.previous, collision_cache.current);
    //     collision_cache.current.clear();

    //     PROFILE_SCOPE("EntityCollision()");
    //     for (auto entity_a : world.View<cmpt::Transform, 
    //                                     cmpt::Collider>()) {
            
    //         auto& atrans = world.GetComponent<cmpt::Transform>(entity_a);
    //         auto& acol = world.GetComponent<cmpt::Collider>(entity_a);
    //         auto abox = utils::GetBoundingBox(atrans, acol);

    //         for (auto entity_b : world.View<cmpt::Transform, cmpt::Collider>()) {
    //             // never interact with self
    //             if (entity_a == entity_b) continue;
                
    //             // do these layer interact?
    //             auto& bcol = world.GetComponent<cmpt::Collider>(entity_b);
    //             if (!data::layer::Interacts(acol.layer, acol.mask, bcol.layer, bcol.mask)) {
    //                 continue;
    //             }


    //             auto& btrans = world.GetComponent<cmpt::Transform>(entity_b);
    //             auto bbox = utils::GetBoundingBox(btrans, bcol);
                
    //             if (!CheckCollisionBoxes(abox, bbox)) continue;

    //             // we have a collision
    //             collision_cache.current.insert(strg::CollisionPair{ entity_a, entity_b });
    //         }
    //     }
    // }
}