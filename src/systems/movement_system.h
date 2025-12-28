
#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::mov {
    inline void ApplyPlayerMovement(strg::Registry& world) {
        PROFILE_SCOPE("ApplyPlayerMovement()");
        for (auto entity : world.View<cmpt::Input, 
                                        cmpt::Transform,
                                        cmpt::Velocity, 
                                        cmpt::Stats>()) {

            if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
                continue;
            }
            
            if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
                continue;
            }

            auto& vel = world.GetComponent<cmpt::Velocity>(entity);
            
            // if player is being knocked back, take away control
            if (world.HasComponent<cmpt::Knockback>(entity)) {
                auto& kb = world.GetComponent<cmpt::Knockback>(entity);
                vel = kb.direction;
                vel.y = 0.0f;
            } else {
                auto& input = world.GetComponent<cmpt::Input>(entity);
                auto& trans = world.GetComponent<cmpt::Transform>(entity);
                auto& stats = world.GetComponent<cmpt::Stats>(entity);
                float speed = stats.move_speed * stats.move_speed_modifier;

                if (world.HasComponent<cmpt::Dash>(entity)) {
                    speed *= stats.dash_speed_modifier;
                }

                vel.x = input.direction.x * speed;
                vel.y = 0.0f;
                vel.z = input.direction.z * speed;
                trans.rotation = utils::GetRotationToLocation(trans.position, input.mouse_world_position);
            }
        }
    }

 

    inline void ApplyAIMovement(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyAIMovement()");
        for (auto entity : world.View<cmpt::AIMoveIntent,
                                        cmpt::Transform, 
                                        cmpt::Velocity,
                                        cmpt::Stats>()) {

            if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
                continue;
            }
            
            if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
                continue;
            }

            auto& vel = world.GetComponent<cmpt::Velocity>(entity);

            // if enemy is being knocked back, take away control
            if (world.HasComponent<cmpt::Knockback>(entity)) {
                auto& kb = world.GetComponent<cmpt::Knockback>(entity);
                vel = kb.direction;
                vel.y = 0.0f;
            } else {
                auto& intent = world.GetComponent<cmpt::AIMoveIntent>(entity);
                auto& trans = world.GetComponent<cmpt::Transform>(entity);
                auto& stats = world.GetComponent<cmpt::Stats>(entity);
           
                vel.x = intent.direction.x * stats.move_speed * stats.move_speed_modifier;
                vel.y = 0.0f;
                vel.z = intent.direction.z * stats.move_speed * stats.move_speed_modifier;

                // rotation
                switch (intent.mode) {
                    case cmpt::AIMoveMode::Lazy: {} // fallthrough
                    case cmpt::AIMoveMode::Random: {
                        if (intent.rotation_complete) continue;
                        
                        intent.rotation_elapsed += delta_time;
                        float amount = intent.rotation_elapsed / intent.rotation_duration;
                        amount = Clamp(amount, 0.0f, 1.0f);
                        amount = utils::EaseInOutQuad(amount);

                        trans.rotation = QuaternionSlerp(
                            intent.start_rotation,
                            utils::GetRotationToDirection(intent.direction),
                            amount
                        );

                        intent.rotation_complete = intent.rotation_elapsed >= intent.rotation_duration;
                        break;
                    }
                    default: {
                        trans.rotation = utils::GetRotationToDirection(intent.direction);
                        break;
                    }
                }
            }
        }
    }
}