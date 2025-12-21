// #ifndef DEBUG
// // #pragma windows
// // #pragma minCRTStartup
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
// #endif

#include "raylib.h"
#include "raymath.h"

#ifdef DEBUG
#include "rlImGui.h"
#include "imgui.h"
#include "debug/debug_window.h"
#include "utils/debug.h"
#endif

#include "data/game/game.h"
#include "resources/assets.h"
#include "systems/systems.h"
#include "components/components.h"

int main() {
    const int screenWidth  = 1920;
    const int screenHeight = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT); // anti-aliasing
    InitWindow(screenWidth, screenHeight, "3D Roguelite");
    InitAudioDevice();
    SetTargetFPS(240);

    #ifdef DEBUG
    //rlImGuiBeginInitImGui();
    rlImGuiSetup(true);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImFont* font = io.Fonts->AddFontFromFileTTF("assets/fonts/consola.ttf", 16.0f);
    if (font == nullptr) {
        std::cout << "font failed to load" << std::endl;
        return 1;
    } else {
        io.FontDefault = font;
    }
    //rlImGuiEndInitImGui();
    #endif

    rsrc::asset::LoadAssets();
    sys::InitWorld();

    float delta_time = 0.0f;

    while (!WindowShouldClose()) {
        delta_time = GetFrameTime();

        UpdateMusicStream(rsrc::asset::bg_music);

        if (IsKeyPressed(KEY_GRAVE)) {
            data::game::g_paused = !data::game::g_paused;
        }

        sys::RunUpdateSystems(delta_time);

        BeginDrawing();
        ClearBackground(Color{30,30,30,255});
        BeginMode3D(sys::camera);
        sys::RunEntityDrawSystems(delta_time);
        EndMode3D();

        sys::RunUIDrawSystems(delta_time);

        if (data::game::g_paused) {
            // draw dark box over entire screen
            DrawRectangle(
                0, 0, 
                GetScreenWidth(), 
                GetScreenHeight(), 
                Color{0,0,0,175}
            );
        }

        // imgui ui
        #ifdef DEBUG
        debug::DrawDebugUI(sys::world, io);
        #endif
   
        EndDrawing();
    }

    rsrc::asset::UnloadAssets();
    CloseAudioDevice();
    #ifdef DEBUG
    rlImGuiShutdown();
    #endif
    CloseWindow();

    #ifdef DEBUG
    PRINT("\tdebug build")
    #endif
    
    #ifdef PROFILER_ENABLED
    PRINT("\tprofiler enabled")
    #endif

    return 0;
}