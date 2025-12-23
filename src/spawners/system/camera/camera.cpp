#include "spawners/system/camera/camera.h"
#include "data/entity.h"

namespace spwn::camera {
    Camera3D camera;

    Camera3D& Camera() {
        camera = {};
        camera.position   = data::cnst::CAMERA_START_POSITION;
        camera.target     = data::cnst::CAMERA_TARGET;
        camera.up         = data::cnst::CAMERA_UP;
        camera.fovy       = data::cnst::CAMERA_FOVY;
        camera.projection = CAMERA_PERSPECTIVE;
        return camera;
    }
}