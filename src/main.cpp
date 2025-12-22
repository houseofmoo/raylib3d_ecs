// #ifndef DEBUG
// // #pragma windows
// // #pragma minCRTStartup
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
// #endif

#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_dark.h"

#ifdef DEBUG
#include "rlImGui.h"
#include "imgui.h"
#include "debug/debug_window.h"
#include "utils/debug.h"
#endif

#include "data/game/game.h"
#include "resources/asset_loader.h"
#include "systems/systems.h"
#include "components/components.h"

int main() {
    const int screenWidth  = 1920;
    const int screenHeight = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT); // anti-aliasing
    InitWindow(screenWidth, screenHeight, "3D Roguelite");
    InitAudioDevice();
    SetTargetFPS(240);

    // raygui
    ::GuiLoadStyleDark();

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

        // check for pause key
        if (IsKeyPressed(KEY_GRAVE)) {
            if (data::g_game.state == data::GameState_E::Paused) {
                // unpausing restores previous game state
                data::g_game.state = data::g_game.prev_state;
            } else {
                // pausing stores current gmae state and sets pause state
                data::g_game.prev_state = data::g_game.state;
                data::g_game.state = data::GameState_E::Paused;
            }
        }

        switch (data::g_game.state) {
            case data::GameState_E::StartUp: {
                // draw startup menu
                break;
            }
            case data::GameState_E::Running: {
                // normal ops
                sys::RunUpdateSystems(delta_time);
                break;
            }
            case data::GameState_E::WeaponSelect: {
                // draw weapon selection
                break;
            }
            case data::GameState_E::StatsScreen: {
                // draw stats screen menu
                break;
            }
            case data::GameState_E::Paused: {
                // draw pause/options menu
                break;
            }
        }

        BeginDrawing();
        ClearBackground(Color{30,30,30,255});
        BeginMode3D(sys::camera);
        sys::RunEntityDrawSystems(delta_time);
        EndMode3D();

        // if (GuiButton(Rectangle{ (int)screenWidth * 0.5f, (int)screenHeight * 0.5f, 120, 30}, GuiIconText(ICON_INFO, "Show Message"))) {
        //     std::cout << "button pressed" << std::endl;
        // }

        GuiStatusBar(Rectangle{ (int)screenWidth * 0.5f, (int)screenHeight * 0.5f, 120, 30}, "Test");
        float val = 5.0f;
        Color c = Color{0, 0, 0, 100};
        //::DrawRectangleRec(Rectangle{ (int)screenWidth * 0.5f, (int)screenHeight * 0.5f, 240, 60}, c);
        ::GuiGroupBox(Rectangle{ (int)screenWidth * 0.5f, (int)screenHeight * 0.5f, 240, 60}, nullptr);
        ::GuiProgressBar(Rectangle{ (int)screenWidth * 0.5f + 60, (int)screenHeight * 0.5f + 30, 120, 30}, nullptr, nullptr, &val, 1.0f, 10.0f);

        sys::RunUIDrawSystems(delta_time);

        if (data::g_game.state == data::GameState_E::Paused) {
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