
#pragma once

#include "raylib.h"
#include "raymath.h"

#include "storage/registry.h"
#include "components/components.h"
#include "components/tags.h"

namespace sys::vel {
    // ApplyMovement
    inline void ApplyVelocity(Storage::Registry& world, const float delta_time) {
        for (auto entity : world.View<cmpt::Velocity, cmpt::Transform>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& vel = world.GetComponent<cmpt::Velocity>(entity);

            trans.position.x += vel.x * delta_time;
            trans.position.y += vel.y * delta_time;
            trans.position.z += vel.z * delta_time;
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
        for (auto entity : world.View<cmpt::RotateInPlace, cmpt::Transform>()) {

            auto& rip = world.GetComponent<cmpt::RotateInPlace>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);

            Quaternion dq = QuaternionFromAxisAngle({0.0f, 1.0f, 0.0f}, rip.speed * delta_time);
            trans.rotation = QuaternionMultiply(dq, trans.rotation);
            trans.rotation = QuaternionNormalize(trans.rotation);
        }
    }
}