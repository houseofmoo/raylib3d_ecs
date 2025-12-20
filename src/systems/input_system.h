#pragma once

#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"
#include "systems/player/player_input.h"
#include "components/components.h"
#include "utils/debug.h"

namespace sys::input {
    inline bool RayPlaneIntersection(const Ray& ray, float planeY, Vector3& outPoint) {
        if (fabs(ray.direction.y) < 0.0001f)
            return false; // Parallel

        float t = (planeY - ray.position.y) / ray.direction.y;
        if (t < 0.0f)
            return false; // Behind camera

        outPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
        return true;
    }

    inline void PlayerInput(Storage::Registry& world, Camera3D& camera) {
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
                world.AddComponent<cmpt::Dash>(entity, cmpt::Dash{ 3.0f, 0.11f });
            }
        }
    }

    inline void AIMoveIntent(Storage::Registry& world) {
        PROFILE_SCOPE("AIMoveIntent()");
        for (auto entity : world.View<cmpt::MoveIntent, cmpt::Transform>()) {
            auto& intent = world.GetComponent<cmpt::MoveIntent>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);

            if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
                continue;
            }
            
            if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
                continue;
            }

            // get player position
            Entity player_id = data::player::g_player.id;
            auto ptrans = world.TryGetComponent<cmpt::Transform>(player_id);
            if (!ptrans) {
                // TODO: set gameover
                PRINT("player does not exist");
                return;
            }

            switch (intent.type) {
                case cmpt::MoveIntentType::Melee: {
                    intent.direction = Vector3Normalize(
                        Vector3Subtract(ptrans->position, trans.position)
                    );
                    intent.direction.y = 0.0f;
                    break;
                }

                case cmpt::MoveIntentType::Ranged: {
                    // move towards player until theyre some distance from them
                    // move away from player if they're too close
                    break;
                }

                case cmpt::MoveIntentType::Random: {
                    Vector3 dir = utils::Direction(trans.position, ptrans->position);
                    dir = Vector3{
                        dir.x + GetRandomValue(-100, 100),
                        dir.y = 0.0f,
                        dir.z + GetRandomValue(-100, 100),
                    };
                    intent.direction = Vector3Normalize(dir);
                    break;
                }

                default: {}
            }
        }
    }
}