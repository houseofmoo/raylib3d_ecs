#include "systems/collisions/entity_collision_system.h"

#include "storage/uniform_gird_2d.h"
#include "components/components.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::col {
    struct BroadphaseIDs {
        static constexpr int kMax = 5000;
        std::array<uint32_t, kMax> dense_to_entity{};
        int count = 0;
    };

    BroadphaseIDs broad;
    strg::Grid2D grid;
    strg::CollisionEnterCache collision_cache;

    void EntityCollision(strg::Registry& world) {
        PROFILE_SCOPE("EntityCollision()");
        
        // set up for next collision check
        std::swap(collision_cache.previous, collision_cache.current);
        collision_cache.current.clear();
        
        // build grid
        grid.clear();
        broad.count = 0;

        // build grid
        for (auto e : world.View<cmpt::Transform, cmpt::Collider>()) {
            if (broad.count >= BroadphaseIDs::kMax) {
                PRINT("too many entities for Grid2D to handle");
                break;
            }

            // TODO: check if this is an explosion (large collider)
            // if so skip it, example:
            if (world.HasComponent<cmpt::Explosion>(e)) continue;

            broad.dense_to_entity[broad.count] = (uint32_t)e;

            auto& t = world.GetComponent<cmpt::Transform>(e);
            grid.insert((Entity)broad.count, t.position.x, t.position.z);
            broad.count++;
        }

        // populate collision_cache
        grid.for_each_candidate_pair([&](Entity da, Entity db) {
            auto a = broad.dense_to_entity[da];
            auto b = broad.dense_to_entity[db];
            
            auto& ca = world.GetComponent<cmpt::Collider>(a);
            auto& cb = world.GetComponent<cmpt::Collider>(b);
            if (!data::layer::InteractsBoth(ca.layer, ca.mask, cb.layer, cb.mask)) return;
        
            auto& ta = world.GetComponent<cmpt::Transform>(a);
            auto& tb = world.GetComponent<cmpt::Transform>(b);
            auto abox = utils::GetBoundingBox(ta, ca);
            auto bbox = utils::GetBoundingBox(tb, cb);
            
            if (CheckCollisionBoxes(abox, bbox)) {
                collision_cache.current.insert(strg::CollisionPair{a, b});
            }
        });


        // TODO: insert a check for explosions specifically
        // example below
        // this should fix the explosions not hitting all enemies
        {
            PROFILE_SCOPE("EntityCollision() - Explosion checks");
            for (auto e : world.View<cmpt::Explosion, cmpt::Transform, cmpt::Collider>()) {
                auto& etrans = world.GetComponent<cmpt::Transform>(e);
                auto& ecol = world.GetComponent<cmpt::Collider>(e);
                auto ebox = utils::GetBoundingBox(etrans, ecol);

                for (auto target : world.View<cmpt::DamageReceiver, cmpt::Transform, cmpt::Collider>()) {
                    if (target == e) continue;

                    auto& tcol = world.GetComponent<cmpt::Collider>(target);
                    if (!data::layer::InteractsBoth(ecol.layer, ecol.mask, tcol.layer, tcol.mask)) continue;

                    auto& ttrans = world.GetComponent<cmpt::Transform>(target);
                    auto tbox = utils::GetBoundingBox(ttrans, tcol);

                    if (CheckCollisionBoxes(ebox, tbox)) {
                        collision_cache.current.insert(strg::CollisionPair{e, target});
                    }
                }
            }
        }
    }
}