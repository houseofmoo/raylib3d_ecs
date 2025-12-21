#pragma once

#include "storage/registry.h"
#include "data/game/game.h"
#include "spawners/enemies/grunt.h"
#include "spawners/enemies/brute.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys {
    inline void SpawnEnemyInterval(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("SpawnEnemyInterval()");
        
        static float spawn_countdown = data::game::g_enemy_spawn_interval;
        spawn_countdown -= delta_time;
        if (spawn_countdown > 0.0f) return;
        
        data::game::g_difficulty_level += 1;
        
        // spawn enemies faster as difficulty increases
        data::game::g_enemy_spawn_interval = 2.5f - (data::game::g_difficulty_level * 0.005f);
        if (data::game::g_enemy_spawn_interval < 0.5f) {
            data::game::g_enemy_spawn_interval = 0.5f;
        }
        spawn_countdown = data::game::g_enemy_spawn_interval;

        if (data::game::g_number_enemies > 500) {
            PRINT("too many enemies to spawn more!");
            return;
        }

        // increase enemy hp based on difficulty
        int enemy_hp = (data::game::g_difficulty_level / 10) + 20;

        // after difficulty 100, start spawning multiple enemies
        int num_to_spawn = (data::game::g_difficulty_level / 100) + 1;

        for (int i = 0; i < num_to_spawn; i++) {
            Vector3 pos = utils::GetRandomValidPosition();

            // 10% chance to spawn brutes
            int roll = GetRandomValue(0, 99);
            if (roll < 50) {
                spwn::enemy::Grunt(world, Vector3{ pos.x, 0.0f, pos.z }, cmpt::MoveIntentType::Random, enemy_hp);
            } else if (roll >= 50 && roll < 80) {
                spwn::enemy::Grunt(world, Vector3{ pos.x, 0.0f, pos.z }, cmpt::MoveIntentType::Lazy, enemy_hp);
            } else if (roll >= 80 && roll < 90) {
                spwn::enemy::Grunt(world, Vector3{ pos.x, 0.0f, pos.z }, cmpt::MoveIntentType::Melee, enemy_hp);
            } else if (roll >= 90 && roll < 97) {
                spwn::enemy::Brute(world, Vector3{ pos.x, 0.0f, pos.z }, cmpt::MoveIntentType::Lazy, enemy_hp);
            } else {
                spwn::enemy::Brute(world, Vector3{ pos.x, 0.0f, pos.z }, cmpt::MoveIntentType::Melee, enemy_hp);
            }
        }
    }
}