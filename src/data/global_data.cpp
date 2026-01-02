#include "data/global_data.h"

namespace gd {
    strg::Tilemap terrain(
        data::cnst::PLAY_AREA.min,
        data::cnst::PLAY_AREA.max
    );

    constinit GameStatus game {
        .difficulty = 0,
        .enemy_spawn_countdown = 0.0f,
        .enemy_spawn_interval = data::cnst::MAX_SPAWN_INTERVAL,
        .enemy_count = 0,
        .entity_count = 0,
    };

    constinit Cheats cheats {
        .god_mode = false,
        .dont_spawn_enemies = false,
        .always_drop_loot = false,
        .never_drop_loot = false,
        .profiler_enabled = false,
    };

    constinit PlayerInfo player {
        .id = 0,
        .position = { 0.0f, 0.0f, 0.0f }
    };

    constinit Vector2 screen_size = {
        .x = 0.0f,
        .y = 0.0f,
    };
}

