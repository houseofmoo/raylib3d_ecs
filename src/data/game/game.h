#pragma once
#include "raylib.h"
#include "data/entity.h"
#include "storage/tilemap.h"

namespace data {
    struct Cheats {
        bool god_mode;
        bool always_drop_loot;
        bool profiler_enabled;
    };

    enum class GameState_E {
        StartScreen,
        NewGame,
        Running,
        StatsScreen,
        Paused,
        Dead,
    };

    struct GameState {
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
            enemy_spawn_interval = data::cnst::MAX_SPAWN_INTERVAL - (difficulty * 0.005f);
            if (enemy_spawn_interval < data::cnst::MIN_SPAWN_INTERVAL) { 
                enemy_spawn_interval = data::cnst::MIN_SPAWN_INTERVAL;
            }
            enemy_spawn_countdown = enemy_spawn_interval;

            // if too many enemies, do not spawn enemies this time
            if (enemy_count > 500) return 0;

            // every 100 difficulty levels, spawn 1 addition enemy
            return (difficulty / 100) + 1;
        }

        int GetModifiedHp(const int base_hp, const float multiplier) const {
            int mod_hp = static_cast<int>((difficulty / 10) + base_hp);
            return static_cast<int>(mod_hp * multiplier);
        }
        
        void Reset() {
            difficulty = 0;
            enemy_spawn_countdown = 0.0f;
            enemy_spawn_interval =data::cnst::MAX_SPAWN_INTERVAL;
            enemy_count = 0;
            entity_count = 0;
        }
    };

    struct WeaponSelectMenu {
        bool show;
        std::array<int, 3> selections;
    };

    extern Cheats g_cheats;
    extern strg::Tilemap g_terrain;
    extern GameState g_game;
    extern WeaponSelectMenu g_weapon_select_menu;
}