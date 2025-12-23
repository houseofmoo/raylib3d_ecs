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
    Grid2D grid;
    strg::CollisionEnterCache collision_cache;

    void EntityCollision(Storage::Registry& world) {
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
            broad.dense_to_entity[broad.count] = (uint32_t)e;

            auto& t = world.GetComponent<cmpt::Transform>(e);
            grid.insert((Entity)broad.count, t.position.x, t.position.z); // <--- dense id
            broad.count++;
        }

        // populate collision_cache
        grid.for_each_candidate_pair([&](Entity da, Entity db) {
            auto a = broad.dense_to_entity[da];
            auto b = broad.dense_to_entity[db];

            if (world.HasComponent<tag::Terrain>(a)) return;
            if (world.HasComponent<tag::Terrain>(b)) return;
            
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
    }
}