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
        .show_weapon_crate_menu = false,
    };
}