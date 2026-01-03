#pragma once

#include "raylib.h"
#include "storage/registry.h"
#include "state/nav_request.h"

namespace sys {
    extern Camera3D camera;
    extern strg::Registry world;

    void StartGame();
    void RunGameSystems(state::NavRequest& nav, const float delta_time);
}