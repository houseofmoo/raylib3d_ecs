#include "raylib.h"
#include "draw/draw_guard.h"

#ifdef DEBUG
#include "rlImGui.h"
#include "imgui.h"
#include "debug/debug_window.h"
#include "utils/debug.h"
#endif

#include "state/game.h"
#include "state/menus/start_menu/start_menu.h"

#include "data/global_data.h"
#include "assets/assets.h"
#include "assets/asset_loader.h"
#include "systems/systems.h"
#include "components/components.h"

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

int main() {
    ::SetConfigFlags(FLAG_MSAA_4X_HINT); // anti-aliasing
    ::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Roguelite");
    ::InitAudioDevice();
    ::SetTargetFPS(240);

    // raygui
    //::GuiLoadStyleDark();
    rgui::LoadStyles();

    #ifdef DEBUG
    //rlImGuiBeginInitImGui();
    ::rlImGuiSetup(true);
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
    
    Game game;
    state::StateContext ctx { 
        .cmd = game.states, 
        .world = sys::world,
        .camera = sys::camera,
        .input = {}, 
        .delta_time = 0.0f,
    };

    asset::LoadAssets();

    // always start at start up screen
    game.states.Push(std::make_unique<state::StartMenu>());
    game.states.ApplyPending();

    gd::screen_size.x = SCREEN_WIDTH;
    gd::screen_size.y = SCREEN_HEIGHT;

    while (!::WindowShouldClose()) {
        gd::screen_size.x = ::GetScreenWidth();
        gd::screen_size.y = ::GetScreenHeight();
        ctx.delta_time = ::GetFrameTime();;

        // get input and updadte context
        ctx.input.Reset();
        if (IsKeyDown(KEY_W)) ctx.input.up = 1.0f;
        if (IsKeyDown(KEY_S)) ctx.input.down = 1.0f;
        if (IsKeyDown(KEY_A)) ctx.input.left = 1.0f;
        if (IsKeyDown(KEY_D)) ctx.input.right = 1.0f;
        ctx.input.dash = IsKeyDown(KEY_SPACE);
        ctx.input.pause = IsKeyPressed(KEY_GRAVE);

        // udpate background music
        ::UpdateMusicStream(asset::bg_music);

        // run state handlers
        game.states.HandleInput(ctx);
        game.states.Update(ctx);
       
        {
            // draw calls
            guard::Drawing d;
            ::ClearBackground(Color{30,30,30,255});
            game.states.Draw(ctx);
            #ifdef DEBUG
            debug::DrawDebugUI(sys::world, io);
            #endif
        }

        game.states.ApplyPending();
    }

    asset::UnloadAssets();
    ::CloseAudioDevice();
    #ifdef DEBUG
    ::rlImGuiShutdown();
    #endif
    ::CloseWindow();

    #ifdef DEBUG
    PRINT("\tdebug build")
    #endif
    
    #ifdef PROFILER_ENABLED
    PRINT("\tprofiler enabled")
    #endif
    return 0;
}