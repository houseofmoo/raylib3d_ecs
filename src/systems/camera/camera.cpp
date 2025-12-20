#include "systems/camera/camera.h"

#include "raymath.h"
#include "components/components.h"
#include "data/entity.h"
#include "utils/debug.h"


namespace sys::cam {
    void CameraMovement(Storage::Registry& world, Camera3D& camera, const float delta_time) {
        PROFILE_SCOPE("CameraMovement()");
        constexpr Vector3 CAMERA_OFFSET = { 0.0f, 35.0f, 15.0f };   // up + back (fixed)
        constexpr Vector3 TARGET_OFFSET = { 0.0f, 0.5f, 0.0f };     // look a bit above feet
        constexpr float FOLLOW_SHARPNESS = 14.0f;                   // bigger = tighter

        Vector3 player_position = Vector3Zero();
         for (auto entity : world.View<tag::Player,
                                        cmpt::Transform>()) {

            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            player_position = trans.position;
            break;
        }

        Vector3 cam_desired_pos = Vector3Add(player_position, CAMERA_OFFSET);
        Vector3 cam_desired_target = Vector3Add(player_position, TARGET_OFFSET);
        float tightness = 1.0f - expf(-FOLLOW_SHARPNESS * delta_time);

        camera.position = Vector3Lerp(camera.position, cam_desired_pos, tightness);
        camera.target   = Vector3Lerp(camera.target, cam_desired_target, tightness);
    }
}