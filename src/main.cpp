#include "main.h"

#include "raylib.h"
// #include "draw/ui/start_screen.h"
// #include "render/ui/death_screen.h"
// #include "render/ui/hud.h"
// #include "render/ui/weapon_select.h"
// #include "render/entities/render_entities.h"
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

    asset::LoadAssets();
    sys::InitWorld();

    Game game;
    state::Input input;
    state::StateContext ctx { 
        .cmd = game.states, 
        .input = input, 
        .world = sys::world,
        .delta_time = 0.0f,
    };

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

// void HandlePause(bool clicked) {
//     static game::GameState_E prev_state;
//     if (::IsKeyPressed(KEY_GRAVE) || clicked) {
//         if (game::g_game.state == game::GameState_E::Paused) {
//             // unpausing restores previous game state
//             game::g_game.state = prev_state;
//         } else {
//             // pausing stores current gmae state and sets pause state
//             prev_state = game::g_game.state;
//             game::g_game.state = game::GameState_E::Paused;
//         }
//     }
// }

// void StateSystems(const float delta_time) {
//      switch (game::g_game.state) {
//         case game::GameState_E::StartScreen: {
//             // start up menu checks
//             // if game started transition to newgame
//             break;
//         }

//         case game::GameState_E::NewGame: {
//             sys::StartGame();
//             game::g_game.state = game::GameState_E::Running;
//             break;
//         }

//         case game::GameState_E::Running: {
//             // normal ops
//             sys::RunGameSystems(delta_time);
//             // if player dies transition to death screen?
//             break;
//         }

//         case game::GameState_E::StatsScreen: {
//             // when player presses TAB show stats screen
//             // transition back to running
//             break;
//         }

//         case game::GameState_E::Paused: {
//             // draw pause/options menu
//             break;
//         }
        
//         case game::GameState_E::Dead: {
//             // show death UI
//             break;
//         }
//     }
// }

// void StateDraws(const float delta_time) {
//     switch (game::g_game.state) {
//         case game::GameState_E::StartScreen: {
//             StartScreen();
//             break;
//         }

//         case game::GameState_E::NewGame: {
//             // nothing to draw here
//             break;
//         }

//         case game::GameState_E::Running: {
//             {
//                 guard::Mode3D _(sys::camera);
//                 RenderEntities(sys::world, delta_time);
//             }
//             HUD(sys::world, delta_time);

//             if (game::g_weapon_select_menu.show) {
//                 WeaponSelect(sys::world);
//             }
//             break;
//         }

//         case game::GameState_E::StatsScreen: {
//             // draw stats screen menu
//             break;
//         }

//         case game::GameState_E::Paused: {
//             {
//                 // Render3D render(sys::camera);
//                 guard::Mode3D _(sys::camera);
//                 RenderEntities(sys::world, delta_time);
//             }
//             HUD(sys::world, delta_time);
            
//             // draw dark box over entire screen
//             DrawRectangle(0, 0, app::g_screen_width, app::g_screen_height, Color{0,0,0,175});

//             if (rgui::Button("Unpause", Vector2{ app::g_screen_width * 0.5f, 150.0f }, rgui::HAlign::Center)) {
//                 HandlePause(true);
//             }
//             break;
//         }

//         case game::GameState_E::Dead: {
//             DeathScreen();
//             break;
//         }
//     }
// }