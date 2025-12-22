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
        delta_time = GetFrameTime();
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        ::UpdateMusicStream(rsrc::asset::bg_music);

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
            case data::GameState_E::StartScreen: {
                // start up menu checks
                // if game started transition to newgame
                break;
            }
            case data::GameState_E::NewGame: {
                // run inits on new game and transition to running
                break;
            }
            case data::GameState_E::Running: {
                // normal ops
                sys::RunUpdateSystems(delta_time);

                // if player dies transition to death screen?
                break;
            }
            case data::GameState_E::WeaponSelect: {
                // when player picks up weapon crate, draw that menu
                // transition back to running
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
        }

        BeginDrawing();
        ClearBackground(Color{30,30,30,255});
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
                    data::g_game.prev_state = data::g_game.state;
                    data::g_game.state = data::GameState_E::NewGame;
                }
                break;
            }
            case data::GameState_E::NewGame: {
                sys::StartGame();
                data::g_game.prev_state = data::g_game.state;
                data::g_game.state = data::GameState_E::Running;
                break;
            }
            case data::GameState_E::Running: {
                ::BeginMode3D(sys::camera);
                sys::RunEntityDrawSystems(delta_time);
                ::EndMode3D();
                sys::RunUIDrawSystems(delta_time);
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

        #ifdef DEBUG
        // imgui ui
        debug::DrawDebugUI(sys::world, io);
        #endif
        ::EndDrawing();

        // Draw(
        //     delta_time,
        //     [](const float dt) {
        //         sys::RunEntityDrawSystems(dt);
        //     },
        //     [](const float dt) {
        //         sys::RunUIDrawSystems(dt);
        //     }
        // );

        // Draw(
        //     [delta_time]() {
        //         sys::RunEntityDrawSystems(delta_time);
        //     },
        //     [delta_time]() {
        //         sys::RunUIDrawSystems(delta_time);
        //     }
        // );

        // #if DEBUG
        // Draw(
        //     [delta_time]() {
        //         sys::RunEntityDrawSystems(delta_time);
        //     },
        //     [delta_time]() {
        //         sys::RunUIDrawSystems(delta_time);
        //     },
        //     io
        // );
        // else
        // Draw(
        //     [delta_time]() {
        //         sys::RunEntityDrawSystems(delta_time);
        //     },
        //     [delta_time]() {
        //         sys::RunUIDrawSystems(delta_time);
        //     }
        // );
        // #endif

        // BeginDrawing();
        // ClearBackground(Color{30,30,30,255});
        // BeginMode3D(sys::camera);
        // sys::RunEntityDrawSystems(delta_time);
        // EndMode3D();

        // // // if (GuiButton(Rectangle{ (int)screenWidth * 0.5f, (int)screenHeight * 0.5f, 120, 30}, GuiIconText(ICON_INFO, "Show Message"))) {
        // // //     std::cout << "button pressed" << std::endl;
        // // // }

        // // GuiStatusBar(Rectangle{ (int)screen_width * 0.5f, (int)screen_height * 0.5f, 120, 30}, "Test");
        // // float val = 5.0f;
        // // Color c = Color{0, 0, 0, 100};
        // // //::DrawRectangleRec(Rectangle{ (int)screenWidth * 0.5f, (int)screenHeight * 0.5f, 240, 60}, c);
        // // ::GuiGroupBox(Rectangle{ (int)screen_width * 0.5f, (int)screen_height * 0.5f, 240, 60}, nullptr);
        // // ::GuiProgressBar(Rectangle{ (int)screen_width * 0.5f + 60, (int)screen_height * 0.5f + 30, 120, 30}, nullptr, nullptr, &val, 1.0f, 10.0f);

        // sys::RunUIDrawSystems(delta_time);

        // if (data::g_game.state == data::GameState_E::Paused) {
        //     // draw dark box over entire screen
        //     DrawRectangle(
        //         0, 0, 
        //         GetScreenWidth(), 
        //         GetScreenHeight(), 
        //         Color{0,0,0,175}
        //     );
        // }

        // // imgui ui
        // #ifdef DEBUG
        // debug::DrawDebugUI(sys::world, io);
        // #endif
   
        // EndDrawing();
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