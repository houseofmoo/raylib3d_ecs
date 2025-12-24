#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace sys {
    extern Camera3D camera;
    extern Storage::Registry world;

    void InitWorld();
    void StartGame();
    void RunUpdateSystems(const float delta_time);
    void RunEntityDrawSystems(const float delta_time);
    void RunUIDrawSystems(const float delta_time);
    void RunWeaponSelectDrawSystem(const int screen_width, const int screen_height);
}