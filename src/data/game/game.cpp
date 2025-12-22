#include "data/game/game.h"
#include "data/entity.h"
#include "raymath.h"

namespace data {
    // int g_difficulty_level = 0;
    // float g_enemy_spawn_interval = 1.5f;
    // int g_number_entities = 0;
    // int g_number_enemies = 0;
    // bool g_paused = false;

    strg::Tilemap g_terrain(
        data::size::PLAY_AREA.min,
        data::size::PLAY_AREA.max
    );

    GameState g_game {
        .prev_state = GameState_E::Running,
        .state = GameState_E::Running,
        .difficulty = 0,
        .enemy_spawn_countdown = 0.0f,
        .enemy_spawn_interval = MAX_SPAWN_INTERVAL,
        .enemy_count = 0,
        .entity_count = 0
    };
}