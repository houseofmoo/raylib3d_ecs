#pragma once

#include "storage/registry.h"
#include "data/game/game.h"
#include "spawners/world/enemies/grunt.h"
#include "spawners/world/enemies/brute.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys {
    inline void SpawnEnemyInterval(Storage::Registry& world, const float delta_time) {
        PROFILE_SCOPE("SpawnEnemyInterval()");
        
        data::g_game.Tick(delta_time);

        // after difficulty 100, start spawning multiple enemies
        int num_to_spawn = data::g_game.GetSpawnEnemies();

        for (int i = 0; i < num_to_spawn; i++) {
            Vector3 pos = utils::GetRandomValidPosition();

            // 10% chance to spawn brutes
            int roll = GetRandomValue(0, 99);
            if (roll < 50) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Random, 
                    data::g_game.GetModifiedHp(data::cnst::GRUNT_MIN_HP, 1.0f)
                );
            } else if (roll >= 50 && roll < 80) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Lazy, 
                    data::g_game.GetModifiedHp(data::cnst::GRUNT_MIN_HP, 1.0f)
                );
            } else if (roll >= 80 && roll < 90) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Melee, 
                    data::g_game.GetModifiedHp(data::cnst::GRUNT_MIN_HP, 1.0f)
                );
            } else if (roll >= 90 && roll < 97) {
                spwn::enemy::Brute(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Lazy, 
                    data::g_game.GetModifiedHp(data::cnst::BRUTE_MIN_HP, 1.0f)
                );
            } else {
                spwn::enemy::Brute(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Melee, 
                    data::g_game.GetModifiedHp(data::cnst::BRUTE_MIN_HP, 1.0f)
                );
            }
        }
    }
}