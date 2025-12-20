
#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "data/player/player.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::mov {
    inline void ApplyPlayerMovement(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyPlayerMovement()");
        for (auto entity : world.View<cmpt::Input, 
                                        cmpt::Transform,
                                        cmpt::Velocity, 
                                        cmpt::Speed>()) {

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
                auto& spd = world.GetComponent<cmpt::Speed>(entity);
                float speed = spd.speed * 
                            spd.speed_multiplier * 
                            spd.dash_multiplier * 
                            data::player::g_player.move_speed_multiplier;

                vel.x = input.direction.x * speed;
                vel.y = 0.0f;
                vel.z = input.direction.z * speed;
                trans.rotation = utils::GetRotationToLocation(trans.position, input.mouse_world_position);
            }
        }
    }

    inline void ApplyAIMovement(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyAIMovement()");
        for (auto entity : world.View<cmpt::MoveIntent,
                                        cmpt::Transform, 
                                        cmpt::Velocity,
                                        cmpt::Speed>()) {

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
                auto& intent = world.GetComponent<cmpt::MoveIntent>(entity);
                auto& trans = world.GetComponent<cmpt::Transform>(entity);
                auto& spd = world.GetComponent<cmpt::Speed>(entity);

                vel.x = intent.direction.x * spd.speed * spd.speed_multiplier * spd.dash_multiplier;
                vel.y = 0.0f;
                vel.z = intent.direction.z * spd.speed * spd.speed_multiplier * spd.dash_multiplier;
                trans.rotation = utils::GetRotationToDirection(trans.position, intent.direction);
            }
        }
    }
}