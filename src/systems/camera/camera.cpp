#include "systems/camera/camera.h"

#include "raymath.h"
#include "components/components.h"
#include "data/entity.h"
#include "utils/debug.h"


namespace sys::cam {
    void CameraMovement(Storage::Registry& world, Camera3D& camera, const float delta_time, const bool shake_cam) {
        PROFILE_SCOPE("CameraMovement()");
        Vector3 position = Vector3Zero();
         for (auto entity : world.View<tag::CameraFollow,
                                        cmpt::Transform>()) {

            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            position = trans.position;
            break;
        }

        Vector3 cam_desired_pos = Vector3Add(position, data::cnst::CAMERA_OFFSET);
        Vector3 cam_desired_target = Vector3Add(position, data::cnst::TARGET_OFFSET);
        float tightness = 1.0f - expf(-data::cnst::CAMERA_FOLLOW_SHARPNESS * delta_time);

        if (shake_cam) {
            Vector3 forward = Vector3Normalize(Vector3Subtract(cam_desired_target, cam_desired_pos));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
            Vector3 up = Vector3Normalize(Vector3CrossProduct(right, forward));

            float shakePhase = (float)GetRandomValue(0, 10000) * 0.01f;
            shakePhase += delta_time * data::cnst::CAMERA_SHAKE_FREQ * 6.2831853f;
            float x = cosf(shakePhase * 1.7f) * data::cnst::CAMERA_SHAKE_STR;
            float y = sinf(shakePhase * 2.3f) * data::cnst::CAMERA_SHAKE_STR;

            Vector3 shakeOffset = Vector3Add(Vector3Scale(right, x), Vector3Scale(up, y));

            cam_desired_pos = Vector3Add(cam_desired_pos, shakeOffset);

            // optional: smaller target shake (or comment out to keep aim stable)
            cam_desired_target = Vector3Add(cam_desired_target, Vector3Scale(shakeOffset, 0.35f));
        }

        camera.position = Vector3Lerp(camera.position, cam_desired_pos, tightness);
        camera.target   = Vector3Lerp(camera.target, cam_desired_target, tightness);
    }
}