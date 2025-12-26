
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
    inline void ApplyVelocity(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyVelocity()");
        for (auto entity : world.View<cmpt::Velocity, cmpt::Transform>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& vel = world.GetComponent<cmpt::Velocity>(entity);

            Vector3 new_pos = Vector3Add(trans.position, Vector3Scale(vel, delta_time));
            
            //// spawning animations are not validated
            // if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
            //     trans.position = new_pos;
            //     continue;
            // }

            auto* col = world.TryGetComponent<cmpt::Collider>(entity);
            float height = (col == nullptr) ? 0.0f : utils::GetEntityHeight(trans.position, col->size);
            if (data::g_terrain.ValidMove(new_pos, height)) {
                trans.position = new_pos;
                continue;
            }
         
            if (world.HasComponent<tag::DestroyOnTerrainCollision>(entity)) {
                world.AddComponent<tag::Destroy>(entity);
            } else if (world.HasComponent<cmpt::AIMoveIntent>(entity)) {
                auto& intent = world.GetComponent<cmpt::AIMoveIntent>(entity);
                intent.stuck = true;
            } else {
                trans.position = utils::ValidateMovePosition(trans.position, new_pos, height);
            }
        }
    }

    // ApplyArchMovement
    inline void ApplyArch(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyArch()");
        for (auto entity : world.View<cmpt::ArchMove, cmpt::Transform>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& arch = world.GetComponent<cmpt::ArchMove>(entity);
            auto* col = world.TryGetComponent<cmpt::Collider>(entity);
            
            arch.elapsed += delta_time;
            float time = 0.0f;
            if (arch.duration > 0.0f) {
                time = Clamp(arch.elapsed / arch.duration, 0.0f, 1.0f);
            } else {
                time = 1.0f;
            } 
            //float time_eased = utils::EaseInOutQuad(time);

            Vector3 new_pos = Vector3Lerp(arch.start, arch.end, time);
            new_pos.y += 4.0f * arch.height * time * (1.0f - time);
            float height = (col == nullptr) ? 0.0f : utils::GetEntityHeight(new_pos, col->size);

            if (data::g_terrain.ValidMove(new_pos, height)) {   
                trans.position = new_pos;
                continue;
            }

            if (world.HasComponent<tag::DestroyOnTerrainCollision>(entity)) {
                world.AddComponent<tag::Destroy>(entity);
            } else {
                trans.position = utils::ValidateMovePosition(trans.position, new_pos, height);
            }
        }
    }

    inline void ApplyRotateInPlace(strg::Registry& world, const float delta_time) {
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