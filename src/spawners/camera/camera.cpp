#include "spawners/camera/camera.h"

namespace spwn::camera {
    Camera3D camera;

    Camera3D& Camera() {
        camera = {};
        camera.position   = { 5.0f, 3.0f, 5.0f };  // will be overridden
        camera.target     = { 0.0f, 1.0f, 0.0f };  // look at cube center
        camera.up         = { 0.0f, 1.0f, 0.0f };
        camera.fovy       = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        return camera;
    }
}