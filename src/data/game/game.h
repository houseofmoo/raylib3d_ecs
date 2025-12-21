#pragma once
#include "raylib.h"
#include "storage/tilemap.h"

namespace data::game {
    extern int g_difficulty_level;
    extern float g_enemy_spawn_interval;
    extern int g_number_entities;
    extern int g_number_enemies;
    extern bool g_paused;

    extern strg::Tilemap terrain;
}