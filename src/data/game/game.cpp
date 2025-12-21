#include "data/game/game.h"
#include "data/entity.h"
#include "raymath.h"

namespace data::game {
    int g_difficulty_level = 0;
    float g_enemy_spawn_interval = 1.5f;
    int g_number_entities = 0;
    int g_number_enemies = 0;
    bool g_paused = false;

    strg::Tilemap terrain(
        data::size::PLAY_AREA.min,
        data::size::PLAY_AREA.max
    );
}