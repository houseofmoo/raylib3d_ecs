#include "systems/camera/camera.h"

#include "raymath.h"
#include "components/components.h"
#include "data/entity.h"
#include "utils/debug.h"


namespace sys::cam {
    void CameraMovement(Storage::Registry& world, Camera3D& camera, const float delta_time, const bool shake_cam) {
        PROFILE_SCOPE("CameraMovement()");
        static float g_camera_yaw = 0.0f;
        static float g_camera_pitch = 0.0f;  // radians
        
        Vector3 position = Vector3Zero();
         for (auto entity : world.View<tag::CameraFollow,
                                        cmpt::Transform>()) {

            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            position = trans.position;
            break;
        }

        Vector2 mouse_delta = GetMouseDelta();
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            const float yaw_sens = 0.005f;
            const float pitch_sens = 0.005f;
            g_camera_yaw += -mouse_delta.x * yaw_sens;
            g_camera_pitch += -mouse_delta.y * pitch_sens; // usually invert Y

            const float pitch_min = -PI/3.0f;  // tweak
            const float pitch_max =  PI/3.0f;  // tweak
            if (g_camera_pitch < pitch_min) g_camera_pitch = pitch_min;
            if (g_camera_pitch > pitch_max) g_camera_pitch = pitch_max;
        } else {
            g_camera_yaw = Lerp(g_camera_yaw, 0.0f, delta_time * 2.0f);
            g_camera_pitch = Lerp(g_camera_pitch, 0.0f, delta_time * 2.0f);
        }

        Vector3 base_cam_offset = data::cnst::CAMERA_OFFSET;
        Vector3 base_target_offset = data::cnst::CAMERA_TARGET_OFFSET;

        
        Quaternion q = QuaternionFromEuler(g_camera_pitch, g_camera_yaw, 0.0f);
        Vector3 rotated_cam_offset = Vector3RotateByQuaternion(base_cam_offset, q);
        Vector3 rotated_target_offset = Vector3RotateByQuaternion(base_target_offset, q);

        Vector3 cam_desired_pos = Vector3Add(position, rotated_cam_offset);
        Vector3 cam_desired_target = Vector3Add(position, rotated_target_offset);
        float tightness = 1.0f - expf(-data::cnst::CAMERA_FOLLOW_SHARPNESS * delta_time);

        if (shake_cam) {
            Vector3 forward = Vector3Normalize(Vector3Subtract(cam_desired_target, cam_desired_pos));
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
            Vector3 up = Vector3Normalize(Vector3CrossProduct(right, forward));

            float shakePhase = (float)GetRandomValue(0, 10000) * 0.01f;
            shakePhase += delta_time * data::cnst::CAMERA_SHAKE_FREQ * 6.2831853f;
            float x = cosf(shakePhase * 1.7f) * data::cnst::CAMERA_SHAKE_STR;
            float y = sinf(shakePhase * 2.3f) * data::cnst::CAMERA_SHAKE_STR;

            Vector3 shake_offset = Vector3Add(Vector3Scale(right, x), Vector3Scale(up, y));
            
            cam_desired_pos = Vector3Add(cam_desired_pos, shake_offset);
            // optional: smaller target shake (or comment out to keep aim stable)
            cam_desired_target = Vector3Add(cam_desired_target, Vector3Scale(shake_offset, 0.35f));
        }

        camera.position = Vector3Lerp(camera.position, cam_desired_pos, tightness);
        camera.target   = Vector3Lerp(camera.target, cam_desired_target, tightness);
    }
}