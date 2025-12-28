#include "systems/input/input_system.h"
#include "raymath.h"
#include "components/components.h"
#include "data/player/player.h"
#include "utils/debug.h"

namespace sys::input {
    bool RayPlaneIntersection(const Ray& ray, float planeY, Vector3& outPoint) {
        if (fabs(ray.direction.y) < 0.0001f) return false; // Parallel

        float t = (planeY - ray.position.y) / ray.direction.y;
        if (t < 0.0f) return false; // Behind camera

        outPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
        return true;
    }

    void PlayerInput(strg::Registry& world, Camera3D& camera) {
        PROFILE_SCOPE("PlayerInput()");
        for (auto entity : world.View<tag::Player, cmpt::Input>()) {

            auto& input = world.GetComponent<cmpt::Input>(entity);

            input.direction = { 0.0f, 0.0f, 0.0f };
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
}