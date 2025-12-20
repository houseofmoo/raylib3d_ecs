#pragma once

#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"

namespace sys {
    extern Camera3D camera;
    extern Storage::Registry world;

    void InitWorld() noexcept;
    void RunUpdateSystems(const float delta_time) noexcept;
    void RunEntityDrawSystems(const float delta_time) noexcept;
    void RunUIDrawSystems(const float delta_time) noexcept;
}