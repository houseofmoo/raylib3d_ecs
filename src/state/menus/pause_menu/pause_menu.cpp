#include "state/menus/pause_menu/pause_menu.h"

#include "raylib.h"
#include "draw/rgui.h"
#include "data/global_data.h"
#include "state/menus/options_menu/options_menu.h"

namespace state {
    DISABLE_UNUSED_WARNINGS

    void PauseMenu::HandleInput(StateContext& ctx) {
        if (IsKeyPressed(KEY_GRAVE)) {
            ctx.cmd.Pop(); // pop ourselves off the stack
        }
    }

    void PauseMenu::Update(StateContext& ctx) {
        // run system updates as needed
    }

    void PauseMenu::Draw(StateContext& ctx) {
        // draw dark box over entire screen
        DrawRectangle(0, 0, gd::screen_size.x, gd::screen_size.y, Color{ 0, 0, 0, 128 });

        if (rgui::Button("Unpause", Vector2{ gd::screen_size.x * 0.5f, 150.0f }, rgui::HAlign::Center)) {
            ctx.cmd.Pop(); // pop us off the stack
        }

        if (rgui::Button("Options", Vector2{ gd::screen_size.x * 0.5f, 200.0f }, rgui::HAlign::Center)) {
            ctx.cmd.Push(std::make_unique<state::OptionsMenu>()); // overlay options
        }
    }
    
    RESTORE_WARNINGS
}
