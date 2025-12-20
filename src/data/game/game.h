#pragma once
#include "raylib.h"
#include "tilemap.h"

namespace data::game {
    extern int g_difficulty_level;
    extern float g_enemy_spawn_interval;
    extern int g_number_entities;
    extern int g_number_enemies;
    extern bool g_paused;

    extern Tilemap terrain;
}