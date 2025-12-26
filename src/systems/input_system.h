#pragma once

#include "raylib.h"
#include "raymath.h"
#include "data/player/player.h"
#include "data/entity.h"
#include "storage/registry.h"
#include "components/components.h"
#include "utils/position.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::input {
    inline bool RayPlaneIntersection(const Ray& ray, float planeY, Vector3& outPoint) {
        if (fabs(ray.direction.y) < 0.0001f) return false; // Parallel

        float t = (planeY - ray.position.y) / ray.direction.y;
        if (t < 0.0f) return false; // Behind camera

        outPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
        return true;
    }

    inline void PlayerInput(strg::Registry& world, Camera3D& camera) {
        PROFILE_SCOPE("PlayerInput()");

        for (auto entity : world.View<tag::Player, 
                                    cmpt::Input>()) {

            auto& input = world.GetComponent<cmpt::Input>(entity);

            input.direction = { 0.0f, 0.0f };
            if (IsKeyDown(KEY_W)) input.direction.z -= 1.0f;
            if (IsKeyDown(KEY_S)) input.direction.z += 1.0f;
            if (IsKeyDown(KEY_A)) input.direction.x -= 1.0f;
            if (IsKeyDown(KEY_D)) input.direction.x += 1.0f;
            input.direction = Vector3Normalize(input.direction);

            // get mouse position on ground plane
            Vector3 mouse_world_position;
            Ray ray = GetMouseRay(GetMousePosition(), camera);
            if (RayPlaneIntersection(ray, 0.0f, mouse_world_position)) {
                input.mouse_world_position = mouse_world_position;
            }

            // dash
            if (IsKeyPressed(KEY_SPACE) && 
                !world.HasComponent<cmpt::Dash>(entity) && 
                !world.HasComponent<cmpt::DashExhausted>(entity)) {
                world.AddComponent<cmpt::Dash>(
                    entity, 
                    cmpt::Dash{ 
                        .multiplier = data::cnst::PLAYER_DASH_RANGE * data::g_player.dash_range_multiplier,
                        .countdown = data::cnst::PLAYER_DASH_DURATION
                     }
                );
            }
        }
    }

    inline void AIMoveIntent(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("AIMoveIntent()");
        for (auto entity : world.View<cmpt::AIMoveIntent, cmpt::Transform>()) {
            auto& intent = world.GetComponent<cmpt::AIMoveIntent>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);

            if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
                continue;
            }
            
            if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
                continue;
            }

            // get player position
            Entity player_id = data::g_player.id;
            auto ptrans = world.TryGetComponent<cmpt::Transform>(player_id);
            if (!ptrans) {
                // TODO: set gameover
                PRINT("player does not exist");
                return;
            }

            switch (intent.mode) {
                case cmpt::AIMoveMode::Melee: {
                    auto& detour = world.GetComponent<cmpt::MeleeMovement>(entity);
                    if (intent.stuck) {
                        // detour in a random direction for 3 seconds
                        detour.detour_countdown = data::cnst::ENEMY_DETOUR_DURATION;
                        detour.detour_direction = utils::DirectionFlattenThenNormalize(
                            trans.position,
                            utils::GetRandomValidPosition()
                        );
                        intent.stuck = false;
                    }

                    if (detour.detour_countdown > 0.0f) {
                        detour.detour_countdown -= delta_time;
                        intent.direction = detour.detour_direction;
                    } else {
                        // moves directly at the player
                        intent.direction = utils::DirectionFlattenThenNormalize(
                            trans.position,
                            ptrans->position
                        );
                    }
                    break;
                }

                case cmpt::AIMoveMode::Ranged: {
                    // move towards player until theyre some distance from them
                    // move away from player if they're too close
                    break;
                }

                case cmpt::AIMoveMode::Lazy: {
                    // make their way in the general direction of the player
                    auto& lazy = world.GetComponent<cmpt::LazyMovement>(entity);
                    lazy.countdown -= delta_time;
                    if (lazy.countdown > 0.0f && !intent.stuck) continue;
                    intent.stuck = false;
                    lazy.countdown = data::cnst::ENEMY_LAZY_RETARGET_TIME;
                    Vector3 dir = utils::Direction(trans.position, ptrans->position);
                    // add a random offset so we dont move directly at the player
                    dir.x += (float)GetRandomValue(-5, 5);
                    dir.z += (float)GetRandomValue(-5, 5);
                    intent.direction = utils::FlattenThenNormalize(dir);
                    intent.SetSmoothRotation(trans.rotation);
                    break;
                }

                case cmpt::AIMoveMode::Random: {
                    auto& ran_dir = world.GetComponent<cmpt::RandomMovement>(entity);
                    ran_dir.countdown -= delta_time;
                    if (ran_dir.countdown > 0.0f && !intent.stuck) continue;
                    intent.stuck = false;
                    ran_dir.countdown = data::cnst::ENEMY_RANDOM_RETARGET_TIME;
                    intent.direction = utils::DirectionFlattenThenNormalize(
                         trans.position,
                        utils::GetRandomValidPosition()
                    );
                    intent.SetSmoothRotation(trans.rotation);
                    break;
                }

                default: {}
            }
        }
    }
}