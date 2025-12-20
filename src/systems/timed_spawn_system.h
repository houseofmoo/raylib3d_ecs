#pragma once

#include "storage/registry.h"
#include "data/game/game.h"
#include "spawners/enemies/grunt.h"
#include "spawners/enemies/brute.h"
#include "utils/debug.h"

namespace sys {
    inline void SpawnEnemyInterval(Storage::Registry& world, const BoundingBox& boundary, const float delta_time) {
        PROFILE_SCOPE("SpawnEnemyInterval()");
        
        static float spawn_countdown = data::game::g_enemy_spawn_interval;
        spawn_countdown -= delta_time;
        if (spawn_countdown > 0.0f) return;
        
        data::game::g_difficulty_level += 1;
        
        // from difficulty 0..100 spawn enemies faster
        data::game::g_enemy_spawn_interval = 1.5f - (data::game::g_difficulty_level * 0.01f);
        if (data::game::g_enemy_spawn_interval < 0.5f) {
            data::game::g_enemy_spawn_interval = 0.5f;
        }
        spawn_countdown = data::game::g_enemy_spawn_interval;

        if (data::game::g_number_enemies > 500) {
            PRINT("too many enemies to spawn more!");
            return;
        }

        // increase enemy hp based on difficulty
        int enemy_hp = (data::game::g_difficulty_level / 8) + 25;

        // after difficulty 100, start spawning multiple enemies
        int num_to_spawn = (data::game::g_difficulty_level / 100) + 1;

        // while (data::game::g_number_enemies < 500) {
        //     float enemy_x = (float)GetRandomValue(boundary.min.x + 1.0f, boundary.max.x - 1.0f);
        //     float enemy_z = (float)GetRandomValue(boundary.min.z + 1.0f, boundary.max.z - 1.0f);
        //     spwn::enemy::Grunt(world, Vector3{ enemy_x, 0.0f, enemy_z }, 500);
        //     data::game::g_number_enemies += 1;
        // }

        for (int i = 0; i < num_to_spawn; i++) {
            float enemy_x = (float)GetRandomValue(boundary.min.x + 1.0f, boundary.max.x - 1.0f);
            float enemy_z = (float)GetRandomValue(boundary.min.z + 1.0f, boundary.max.z - 1.0f);

            // 10% chance to spawn brutes
            int roll = GetRandomValue(0, 99);
            if (roll >= 90) {
                spwn::enemy::Brute(world, Vector3{ enemy_x, 0.0f, enemy_z }, enemy_hp);
            } else {
                spwn::enemy::Grunt(world, Vector3{ enemy_x, 0.0f, enemy_z }, enemy_hp);
            }
        }
    }
}