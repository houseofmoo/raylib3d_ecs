
#pragma once

#include "raylib.h"
#include "raymath.h"

#include "storage/registry.h"
#include "components/components.h"
#include "utils/position_helpers.h"
#include "utils/debug.h"

namespace sys::vel {
    // ApplyMovement
    inline void ApplyVelocity(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyVelocity()");
        for (auto entity : world.View<cmpt::Velocity, cmpt::Transform>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& vel = world.GetComponent<cmpt::Velocity>(entity);

            Vector3 delta = Vector3Scale(vel, delta_time);

            // special case entities that are destroyed on terrain contact
            if (world.HasComponent<tag::DestroyOnTerrainCollision>(entity)) {
                Vector3 new_pos = Vector3Add(trans.position, delta);
                if (data::game::terrain.IsBlockedWorld(new_pos.x, new_pos.z)) {
                    world.AddComponent<tag::Destroy>(entity);
                    continue;
                }
            }

            // only spawning animations have a Y velocity
            if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
                trans.position = Vector3Add(trans.position, delta);
                continue;
            }

            utils::MoveAndSlideTerrain(trans.position, delta);

            // // test X position, apply if valid
            // Vector3 try_x = trans.position;
            // try_x.x += delta.x;
            // if (!data::game::terrain.IsBlockedWorld(try_x.x, try_x.z)) {
            //     trans.position.x = try_x.x;
            // } else {
            //     vel.x = 0.0f;
            // }

            // // test Z position, apply if valid
            // Vector3 try_z = trans.position;
            // try_z.z += delta.z;
            // if (!data::game::terrain.IsBlockedWorld(try_z.x, try_z.z)) {
            //     trans.position.z = try_z.z;
            // } else {
            //     vel.z = 0.0f;
            // }

            // // apply Y
            // trans.position.y += delta.y;
        }
    }

    inline float EaseInOutQuad(float t) {
        if (t < 0.5f) {
            return 2.0f * t * t;
        }
        return 1.0f - powf(-2.0f * t + 2.0f, 2.0f) * 0.5f;
    }

    // ApplyArchMovement
    inline void ApplyArch(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyArch()");
        // this are not moving in normal horizontal space, they're arching
        // through the air and do not have a cmpt::Velocty and instead have
        // a cmpt::ArchMove
        for (auto entity : world.View<cmpt::ArchMove, cmpt::Transform>()) {

            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& arch = world.GetComponent<cmpt::ArchMove>(entity);

            arch.elapsed += delta_time;
            float u = Clamp(arch.elapsed, 0.0f, arch.duration);
            float u_eased = EaseInOutQuad(u);

            Vector3 new_pos = Vector3Lerp(arch.start, arch.end, u_eased);
            new_pos.y += 4.0f * arch.height * u * (1.0f - u);
            trans.position = new_pos;
        }
    }

    inline void ApplyRotateInPlace(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyRotateInPlace()");
        for (auto entity : world.View<cmpt::RotateInPlace, cmpt::Transform>()) {

            auto& rip = world.GetComponent<cmpt::RotateInPlace>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);

            Quaternion dq = QuaternionFromAxisAngle({0.0f, 1.0f, 0.0f}, rip.speed * delta_time);
            trans.rotation = QuaternionMultiply(dq, trans.rotation);
            trans.rotation = QuaternionNormalize(trans.rotation);
        }
    }
}