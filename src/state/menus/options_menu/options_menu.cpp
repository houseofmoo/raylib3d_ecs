#include "state/menus/options_menu/options_menu.h"

#include "raylib.h"
#include "draw/rgui.h"
#include "data/global_data.h"
#include "utils/debug.h"

namespace state {
    DISABLE_UNUSED_WARNINGS
    bool show_text = false;

    void OptionsMenu::HandleInput(StateContext& ctx) {
    }

    void OptionsMenu::Update(StateContext& ctx) {
    }

    void OptionsMenu::Draw(StateContext& ctx) {
        // draw dark box over entire screen
        DrawRectangle(
            gd::screen_size.x * 0.5f - 300,
            0,
            600,
            400,
            Color{255,0,0,128}
        );
        rgui::Label("Options", Vector2{ gd::screen_size.x * 0.5f, 100.0f }, rgui::HAlign::Center);

        if (rgui::Button("Close", Vector2{ gd::screen_size.x * 0.5f, 200.0f }, rgui::HAlign::Center)) {
            ctx.cmd.Pop();// close options
        }
    }
    RESTORE_WARNINGS
}
