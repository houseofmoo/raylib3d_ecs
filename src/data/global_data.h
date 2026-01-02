#pragma once 

#include "raylib.h"
#include "data/entity.h"
#include "storage/tilemap.h"

namespace gd {
    struct Cheats {
        bool god_mode;
        bool dont_spawn_enemies;
        bool always_drop_loot;
        bool never_drop_loot;
        bool profiler_enabled;
    };

    struct GameStatus {
        int difficulty;
        float enemy_spawn_countdown;
        float enemy_spawn_interval;
        int enemy_count;
        int entity_count;
    };

    struct PlayerInfo {
        Entity id;
        Vector3 position;
    };

    extern Cheats cheats;
    extern strg::Tilemap terrain;
    extern GameStatus game;
    extern PlayerInfo player;
    extern Vector2 screen_size;
}