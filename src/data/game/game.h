#pragma once
#include "raylib.h"
#include "storage/tilemap.h"

namespace data {
    constexpr float MAX_SPAWN_INTERVAL = 2.5f;
    constexpr float MIN_SPAWN_INTERVAL = 0.5f;

    enum class GameState_E {
        StartScreen,
        NewGame,
        Running,
        WeaponSelect,
        StatsScreen,
        Paused,
        Dead,
    };

    struct GameState {
        GameState_E prev_state;
        GameState_E state;
        int difficulty;
        float enemy_spawn_countdown;
        float enemy_spawn_interval;
        int enemy_count;
        int entity_count;

        void Tick(float delta_time) {
            enemy_spawn_countdown -= delta_time;
        }

        int GetSpawnEnemies() {
            // if not ready to spawn enemies, return 0 enemies
            if (enemy_spawn_countdown > 0.0f) return 0;
            
            // increase difficulty
            difficulty += 1;
            
            // reduce spawn interval
            enemy_spawn_interval = MAX_SPAWN_INTERVAL - (difficulty * 0.005f);
            if (enemy_spawn_interval < MIN_SPAWN_INTERVAL) { 
                enemy_spawn_interval = MIN_SPAWN_INTERVAL;
            }
            enemy_spawn_countdown = enemy_spawn_interval;

            // if too many enemies, do not spawn enemies this time
            if (enemy_count > 500) return 0;

            // every 100 difficulty levels, spawn 1 addition enemy
            return (difficulty / 100) + 1;
        }

        int GetEnemBaseHp(const float multiplier) const {
            int base_hp = static_cast<int>((difficulty / 10) + 20);
            return static_cast<int>(base_hp * multiplier);
        }
        
        void Reset() {
            difficulty = 0;
            enemy_spawn_countdown = 0.0f;
            enemy_spawn_interval = MAX_SPAWN_INTERVAL;
            enemy_count = 0;
            entity_count = 0;
        }
    };



    extern strg::Tilemap g_terrain;
    extern GameState g_game;
}