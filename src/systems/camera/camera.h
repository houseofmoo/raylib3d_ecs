#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace sys::cam {
    void CameraMovement(
        Storage::Registry& world, 
        Camera3D& camera, 
        const float delta_time,
        const bool shake_cam
    );
}