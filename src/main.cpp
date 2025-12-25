#include "main.h"

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

int main() {
    ::SetConfigFlags(FLAG_MSAA_4X_HINT); // anti-aliasing
    ::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Roguelite");
    ::InitAudioDevice();
    ::SetTargetFPS(240);

    // raygui
    //::GuiLoadStyleDark();
    ::GuiLoadStyleBoxOnBox();

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

    rsrc::asset::LoadAssets();
    sys::InitWorld();

    float delta_time = 0.0f;
    int screen_width = SCREEN_WIDTH;
    int screen_height = SCREEN_HEIGHT;

    while (!::WindowShouldClose()) {
        delta_time = ::GetFrameTime();
        screen_width = ::GetScreenWidth();
        screen_height = ::GetScreenHeight();

        ::UpdateMusicStream(rsrc::asset::bg_music);

        // check for pause key
        HandlePause(false);

        StateSystems(delta_time);
       
        // DRAWING ONLY BELOW HERE
        ::BeginDrawing();
        ::ClearBackground(Color{30,30,30,255});
        StateDraws(delta_time, screen_width, screen_height);
        #ifdef DEBUG
            // imgui ui
            debug::DrawDebugUI(sys::world, io);
        #endif
        ::EndDrawing();
    }

    rsrc::asset::UnloadAssets();
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

void HandlePause(bool clicked) {
    static data::GameState_E prev_state;
    if (::IsKeyPressed(KEY_GRAVE) || clicked) {
        if (data::g_game.state == data::GameState_E::Paused) {
            // unpausing restores previous game state
            data::g_game.state = prev_state;
        } else {
            // pausing stores current gmae state and sets pause state
            prev_state = data::g_game.state;
            data::g_game.state = data::GameState_E::Paused;
        }
    }
}

void StateSystems(const float delta_time) {
     switch (data::g_game.state) {
        case data::GameState_E::StartScreen: {
            // start up menu checks
            // if game started transition to newgame
            break;
        }

        case data::GameState_E::NewGame: {
            sys::StartGame();
            data::g_game.state = data::GameState_E::Running;
            break;
        }

        case data::GameState_E::Running: {
            // normal ops
            sys::RunUpdateSystems(delta_time);
            // if player dies transition to death screen?
            break;
        }

        case data::GameState_E::StatsScreen: {
            // when player presses TAB show stats screen
            // transition back to running
            break;
        }

        case data::GameState_E::Paused: {
            // draw pause/options menu
            break;
        }
        
        case data::GameState_E::Dead: {
            // show death UI
            break;
        }
    }
}

void StateDraws(const float delta_time, const int screen_width, const int screen_height) {
    switch (data::g_game.state) {
        case data::GameState_E::StartScreen: {
            // draw dark box over entire screen
            ::DrawRectangle(0, 0, screen_width, screen_height, Color{0,0,0,175} );

            // draw title
            Font font = ::GuiGetFont();
            int width = ::MeasureText("BOX ON BOX CRIME", font.baseSize);
            ::GuiDrawText(
                "BOX ON BOX CRIME", 
                Rectangle{ 
                    (screen_width * 0.5f) - (width * 0.5f), 
                    100, 
                    (float)width, 
                    50 
                }, 
                TEXT_ALIGN_CENTER, WHITE
            );

            if (::GuiButton(Rectangle{ (screen_width * 0.5f) - 50.0f, 150.0f, 100.0f, 50.0f }, "Start")) {
                data::g_game.state = data::GameState_E::NewGame;
            }
            break;
        }

        case data::GameState_E::NewGame: {
            // nothing to draw here
            break;
        }

        case data::GameState_E::Running: {
            ::BeginMode3D(sys::camera);
            sys::RunEntityDrawSystems(delta_time);
            ::EndMode3D();
            sys::RunUIDrawSystems(delta_time);

            if (data::g_weapon_select_menu.show) {
                sys::RunWeaponSelectDrawSystem(screen_width, screen_height);
            }
            break;
        }

        case data::GameState_E::StatsScreen: {
            // draw stats screen menu
            break;
        }

        case data::GameState_E::Paused: {
            // draw dark box over entire screen
            DrawRectangle(0, 0, screen_width, screen_height, Color{0,0,0,175});

            if (::GuiButton(Rectangle{ (screen_width * 0.5f) - 50.0f, 150.0f, 100.0f, 50.0f }, "Unpause")) {
                HandlePause(true);
            }
            break;
        }

        case data::GameState_E::Dead: {
            // draw dark box over entire screen
            ::DrawRectangle(0, 0, screen_width, screen_height, Color{0,0,0,175} );

            // draw title
            Font font = ::GuiGetFont();
            int width = ::MeasureText("YOU DIED! PLAY AGAIN?", font.baseSize);
            ::GuiDrawText(
                "YOU DIED! PLAY AGAIN?", 
                Rectangle{ 
                    (screen_width * 0.5f) - (width * 0.5f), 
                    100, 
                    (float)width, 
                    50 
                }, 
                TEXT_ALIGN_CENTER, WHITE
            );

            if (::GuiButton(Rectangle{ (screen_width * 0.5f) - 50.0f, 150.0f, 100.0f, 50.0f }, "Start")) {
                data::g_game.state = data::GameState_E::NewGame;
            }
            break;
        }
    }
}