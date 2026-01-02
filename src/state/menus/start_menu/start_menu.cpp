#include "state/menus/start_menu/start_menu.h"

#include <memory>
#include "raylib.h"
#include "draw/rgui.h"
#include "data/global_data.h"
#include "state/menus/options_menu/options_menu.h"
#include "state/play/play.h"
#include "utils/debug.h"

namespace state {
    DISABLE_UNUSED_WARNINGS

    void StartMenu::HandleInput(StateContext& ctx) {}

    void StartMenu::Update(StateContext& ctx) {}

    void StartMenu::Draw(StateContext& ctx) {
        // draw dark box over entire screen
        //::DrawRectangle(0, 0, gd::screen_width, gd::screen_height, Color{0,0,0,175} );

        // draw title
        rgui::Label("BOX ON BOX CRIME", Vector2{ gd::screen_size.x * 0.5f, 100.0f }, rgui::HAlign::Center);
        if (rgui::Button("Start", Vector2{ gd::screen_size.x * 0.5f, 150.0f }, rgui::HAlign::Center)) {
            ctx.cmd.Pop(); // pop self off the stack
            ctx.cmd.Push(std::make_unique<state::Play>()); // add game to stack
        }

        if (rgui::Button("Options", Vector2{ gd::screen_size.x * 0.5f, 200.0f }, rgui::HAlign::Center)) {
            ctx.cmd.Push(std::make_unique<state::OptionsMenu>()); // overlay options
        }
    }
    RESTORE_WARNINGS
}
