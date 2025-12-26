#include "data/game/game.h"
#include "data/entity.h"
#include "raymath.h"

namespace data {
    strg::Tilemap g_terrain(
        data::cnst::PLAY_AREA.min,
        data::cnst::PLAY_AREA.max
    );

    constinit GameState g_game {
        .state = GameState_E::StartScreen,
        .difficulty = 0,
        .enemy_spawn_countdown = 0.0f,
        .enemy_spawn_interval = data::cnst::MAX_SPAWN_INTERVAL,
        .enemy_count = 0,
        .entity_count = 0,
    };

    constinit WeaponSelectMenu g_weapon_select_menu {
        .show = false,
        .selections = {-1, -1, -1 }
    };

    constinit Cheats g_cheats {
        .god_mode = false,
        .always_drop_loot = false,
        .never_drop_loot = false,
        .profiler_enabled = false,
    };
}