
#pragma once

#include "raylib.h"
#include "raymath.h"

#include "storage/registry.h"
#include "components/components.h"
#include "data/game/game.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys::vel {
    // ApplyMovement
    inline void ApplyVelocity(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyVelocity()");
        for (auto entity : world.View<cmpt::Velocity, cmpt::Transform>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& vel = world.GetComponent<cmpt::Velocity>(entity);

            Vector3 delta = Vector3Scale(vel, delta_time);

            // projectiles are allow it to travel over low terrain
            if (world.HasComponent<tag::Projectile>(entity)) {
                if (!utils::MoveOverTerrainProjectile(trans.position, delta)) {\
                    world.AddComponent<tag::Destroy>(entity);
                }
                continue;
            }

            // special case entities that are destroyed on terrain or low terrain contact
            if (world.HasComponent<tag::DestroyOnTerrainCollision>(entity)) {
                Vector3 new_pos = Vector3Add(trans.position, delta);
                if (data::g_terrain.IsBlockedWorld(new_pos.x, new_pos.z)) {
                    world.AddComponent<tag::Destroy>(entity);
                    continue;
                }
            }

            // spawning animations are not modified
            if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
                trans.position = Vector3Add(trans.position, delta);
                continue;
            }

            // all over units movement handled here
            utils::MoveAndSlideTerrain(trans.position, delta);

            // if direction attempted to move is invalid and AI, flag as stuck
            if (world.HasComponent<cmpt::AIMoveIntent>(entity)) {
                Vector3 new_pos = Vector3Add(trans.position, delta);
                if (data::g_terrain.IsBlockedWorld(new_pos.x, new_pos.z)) {
                    auto& intent = world.GetComponent<cmpt::AIMoveIntent>(entity);
                    intent.stuck = true;
                }
            }
        }
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
            float u_eased = utils::EaseInOutQuad(u);

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