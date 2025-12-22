#pragma once

#include "raylib.h"
#include "debug/debug_window.h"
#include "systems/systems.h"

template <class Fn, class Fn2>
void Draw(Fn&& _3d_draw, Fn2&& ui_draw, ImGuiIO& io) {
    BeginDrawing();
    ClearBackground(Color{30,30,30,255});
    BeginMode3D(sys::camera);
    
    _3d_draw();

    EndMode3D();
    
    ui_draw();
    
    #ifdef DEBUG
    // imgui ui
    debug::DrawDebugUI(sys::world, io);
    #endif

    EndDrawing();
}

template <class Fn, class Fn2>
void Draw(Fn&& _3d_draw, Fn2&& ui_draw) {
    BeginDrawing();
    ClearBackground(Color{30,30,30,255});
    BeginMode3D(sys::camera);
    
    _3d_draw();

    EndMode3D();
    
    ui_draw();

    EndDrawing();
}