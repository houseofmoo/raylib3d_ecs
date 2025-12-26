#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"
#include "components/components.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys {
    inline void SpawnAnimation(strg::Registry& world) {
        PROFILE_SCOPE("SpawnAnimation()");
        for (auto enemy : world.View<cmpt::SpawnAnimation,
                                    cmpt::Transform,
                                    cmpt::Velocity>()) {
            
            auto& anim = world.GetComponent<cmpt::SpawnAnimation>(enemy);
            auto& etrans = world.GetComponent<cmpt::Transform>(enemy);
            auto& evel = world.GetComponent<cmpt::Velocity>(enemy);

            if (Vector3Distance(etrans.position, anim.end_position) <= 1.0f) {
                etrans.position.y = anim.end_position.y;
                evel.y = 0.0f;
                world.RemoveComponent<cmpt::SpawnAnimation>(enemy);
                continue;
            }

            // downwards velocity during spawn animation
            evel.x = 0.0f;
            evel.y = -30.0f;
            evel.z = 0.0f;
        }
    }

    inline void DeathAnimation(strg::Registry& world, float delta_time) {
        PROFILE_SCOPE("DeathAnimation()");
        for (auto enemy : world.View<cmpt::DeathAnimation,
                                    cmpt::Transform,
                                    cmpt::Velocity>()) {
            // turn off death animation for now
            world.RemoveComponent<cmpt::DeathAnimation>(enemy);
            world.AddComponent<tag::Destroy>(enemy);

            // auto& anim = world.GetComponent<cmpt::DeathAnimation>(enemy);
            // auto& etrans = world.GetComponent<cmpt::Transform>(enemy);
            // auto& evel = world.GetComponent<cmpt::Velocity>(enemy);

            // anim.duration -= delta_time;
            // if (anim.duration < 0.0f) {
            //     world.RemoveComponent<cmpt::DeathAnimation>(enemy);
            //     world.AddComponent<tag::Destroy>(enemy);
            //     continue;
            // }

            // // TODO: this causes them to fall north, need to fix it so they fall away from 
            // // bullet that damaged them...maybe

            // // zero out all velocity during death animation
            // evel.x = 0.0f;
            // evel.y = 0.0f;
            // evel.z = 0.0f;

            // // rotate from feet to face upward
            // auto& col = world.GetComponent<cmpt::Collider>(enemy);
            // Vector3 local_offset = { 0.0f, -col.size.y / 2.0f, 0.0f }; // move pivot to feet
            // Vector3 world_offset = Vector3Add(
            //     etrans.position,
            //     Vector3RotateByQuaternion(local_offset, etrans.rotation)
            // );

            // Quaternion dquat = QuaternionFromAxisAngle({1.0f, 0.0f, 0.0f}, -PI/1.2 * GetFrameTime());
            // Vector3 rel = Vector3Subtract(etrans.position, world_offset);
            // rel = Vector3RotateByQuaternion(rel, dquat);
            // etrans.position = Vector3Add(world_offset, rel);

            // etrans.rotation = QuaternionMultiply(dquat, etrans.rotation);
            // etrans.rotation = QuaternionNormalize(etrans.rotation);
        }
    }
}