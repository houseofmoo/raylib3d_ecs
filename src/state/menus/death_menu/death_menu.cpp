#include "state/menus/death_menu/death_menu.h"

#include <memory>
#include "raylib.h"
#include "draw/rgui.h"
#include "data/global_data.h"
#include "state/play/play.h"
#include "utils/debug.h"

namespace state {
    DISABLE_UNUSED_WARNINGS

    void DeathMenu::HandleInput(StateContext& ctx) {}

    void DeathMenu::Update(StateContext& ctx) {}

    void DeathMenu::Draw(StateContext& ctx) {
        // draw dark box over entire screen
        ::DrawRectangle(0, 0, gd::screen_size.x, gd::screen_size.y, Color{0,0,0,175} );

        // draw title
        rgui::Label("YOU DIED! PLAY AGAIN?", Vector2{ gd::screen_size.x * 0.5f, 100 }, rgui::HAlign::Center);
        if (rgui::Button("Start", Vector2{ gd::screen_size.x * 0.5f, 150.0f }, rgui::HAlign::Center)) {
            ctx.cmd.Pop();
            ctx.cmd.Push(std::make_unique<state::Play>());
        }

        if (rgui::Button("Exit", Vector2{ gd::screen_size.x * 0.5f, 200.0f }, rgui::HAlign::Center)) {
            // TODO: Exit game
        }
    }
    RESTORE_WARNINGS
}
