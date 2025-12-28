#pragma once

#include "storage/registry.h"
#include "data/game/game.h"
#include "spawners/world/enemies/enemies.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys {
    inline void SpawnEnemyInterval(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("SpawnEnemyInterval()");

        if (data::g_cheats.dont_spawn_enemies) return;
        data::g_game.Tick(delta_time);

        // after difficulty 100, start spawning multiple enemies
        int num_to_spawn = data::g_game.GetSpawnEnemies();

        for (int i = 0; i < num_to_spawn; i++) {
            Vector3 pos = utils::GetRandomValidPosition();
            int roll = GetRandomValue(0, 99);

            if (roll < 35) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Random, 
                    data::g_game.GetModifiedHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 35  && roll < 55) {
                spwn::enemy::GruntWithAPistol(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Random, 
                    data::g_game.GetModifiedHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 55 && roll < 75) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Lazy, 
                    data::g_game.GetModifiedHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 75 && roll < 90) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Melee, 
                    data::g_game.GetModifiedHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 90 && roll < 97) {
                spwn::enemy::Brute(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Lazy, 
                    data::g_game.GetModifiedHp(data::cnst::BRUTE_MIN_HP, data::cnst::BRUTE_HP_MULTIPLIER)
                );
            } else {
                spwn::enemy::Brute(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Melee, 
                    data::g_game.GetModifiedHp(data::cnst::BRUTE_MIN_HP, data::cnst::BRUTE_HP_MULTIPLIER)
                );
            }
        }
    }
}