#include "state/play/play.h"

#include "draw/entities/draw_entities.h"
#include "draw/draw_guard.h"
#include "draw/hud/hud.h"
#include "state/menus/pause_menu/pause_menu.h"
#include "state/menus/player_levelup/player_levelup.h"
#include "state/menus/death_menu/death_menu.h"
#include "state/menus/weapon_select/weapon_select.h"
#include "state/nav_request.h"
#include "systems/systems.h"
#include "utils/debug.h"

namespace state {
    DISABLE_UNUSED_WARNINGS

    void Play::OnEnter() {
        sys::StartGame();
    }

    void Play::HandleInput(StateContext& ctx) {
        // check for pause
        if (IsKeyPressed(KEY_GRAVE)) {
            ctx.cmd.Push(std::make_unique<state::PauseMenu>());
        }
    }

    void Play::Update(StateContext& ctx) {
        NavRequest nav;
        sys::RunGameSystems(nav, ctx.delta_time);

        nav.PrioritizeRequests();
        for (auto req : nav) {
            switch (req) {
                case NavRequestKind::DeathMenu: {
                    // if we have a death menu request, ignore all other requests
                    ctx.cmd.Pop();
                    ctx.cmd.Push(std::make_unique<state::DeathMenu>());
                    return;
                }

                case NavRequestKind::PlayerLevelup: {
                    ctx.cmd.Push(std::make_unique<state::PlayerLevelupMenu>());
                    break;
                }

                case NavRequestKind::WeaponSelectMenu: {
                    ctx.cmd.Push(std::make_unique<state::WeaponSelectMenu>());
                    break;
                }

                case NavRequestKind::None: break;
                default: { PRINT("unexpected state transition request"); break; }
            }
        }

    }

    void Play::Draw(StateContext& ctx) {
        {
            guard::Mode3D _(sys::camera);
            draw::DrawEntities(ctx.world, ctx.delta_time);
        }

        draw::DrawHUD(ctx.world, ctx.delta_time);
    }

    RESTORE_WARNINGS
}
