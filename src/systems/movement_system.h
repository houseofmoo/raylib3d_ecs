
#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "components/tags.h"
#include "data/player/player.h"
#include "utils/rl_utils.h"

namespace sys::mov {
    inline void ApplyDash(Storage::Registry& world, const float delta_time) {
        // for (auto entity : world.View<cmpt::Dash, cmpt::Speed>()) {

        //     auto& spd = world.GetComponent<cmpt::Speed>(entity);
        //     auto& dash = world.GetComponent<cmpt::Dash>(entity);

        //     dash.countdown -= delta_time;
        //     if (dash.countdown <= 0.0f) {
        //         world.RemoveComponent<cmpt::Dash>(entity);
        //         world.AddComponent<cmpt::DashExhausted>(entity, cmpt::DashExhausted{ 1.5f });
        //         spd.dash_multiplier = 1.0f;
        //         continue;
        //     }
        //     spd.dash_multiplier = dash.multiplier;
        // }
    }

    inline void ApplyPlayerMovement(Storage::Registry& world, const float delta_time) {
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

            auto& input = world.GetComponent<cmpt::Input>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& vel = world.GetComponent<cmpt::Velocity>(entity);
            trans.rotation = utils::GetRotationToLocation(trans.position, input.mouse_world_position);

            if (world.HasComponent<cmpt::Knockback>(entity)) {
                auto& kb = world.GetComponent<cmpt::Knockback>(entity);
                vel = kb.direction;
                vel.y = 0.0f;
            } else {
                auto& spd = world.GetComponent<cmpt::Speed>(entity);
                float speed = spd.speed * 
                            spd.speed_multiplier * 
                            spd.dash_multiplier * 
                            data::player::g_player.move_speed_multiplier;

                vel.x = input.direction.x * speed;
                vel.y = 0.0f;
                vel.z = input.direction.z * speed;
            }
        }
    }

    inline void ApplyAIMovement(Storage::Registry& world, const float delta_time) {
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

            auto& intent = world.GetComponent<cmpt::MoveIntent>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& vel = world.GetComponent<cmpt::Velocity>(entity);
            auto& spd = world.GetComponent<cmpt::Speed>(entity);

            vel.x = intent.direction.x * spd.speed * spd.speed_multiplier * spd.dash_multiplier;
            vel.y = 0.0f;
            vel.z = intent.direction.z * spd.speed * spd.speed_multiplier * spd.dash_multiplier;
            trans.rotation = utils::GetRotationToDirection(trans.position, intent.direction);
        }
    }
}