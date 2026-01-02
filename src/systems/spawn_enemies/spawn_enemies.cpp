#include "systems/spawn_enemies/spawn_enemies.h"

#include "data/global_data.h"
#include "spawners/world/enemies/enemies.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys {
    int EnemiesToSpawn() {
        // if not ready to spawn enemies, return 0 enemies
        if (gd::game.enemy_spawn_countdown > 0.0f) return 0;
        
        // increase difficulty
        gd::game.difficulty += 1;
        
        // reduce spawn interval
        gd::game.enemy_spawn_interval = data::cnst::MAX_SPAWN_INTERVAL - (gd::game.difficulty * 0.005f);
        if (gd::game.enemy_spawn_interval < data::cnst::MIN_SPAWN_INTERVAL) { 
            gd::game.enemy_spawn_interval = data::cnst::MIN_SPAWN_INTERVAL;
        }
        gd::game.enemy_spawn_countdown = gd::game.enemy_spawn_interval;

        // if too many enemies, do not spawn enemies this time
        if (gd::game.enemy_count > 1000) return 0;

        // every 100 difficulty levels, spawn 1 addition enemy
        return (gd::game.difficulty / 100) + 1;
    }

    int GetEnemyHp(const int base_hp, const float multiplier) {
        int mod_hp = static_cast<int>((gd::game.difficulty / 10) + base_hp);
        return static_cast<int>(mod_hp * multiplier);
    }

    void SpawnEnemyInterval(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("SpawnEnemyInterval()");

        if (gd::cheats.dont_spawn_enemies) return;
        gd::game.enemy_spawn_countdown -= delta_time;

        // after difficulty 100, start spawning multiple enemies
        int num_to_spawn = EnemiesToSpawn();

        for (int i = 0; i < num_to_spawn; i++) {
            Vector3 pos = utils::GetRandomValidPosition();
            int roll = GetRandomValue(0, 99);

            if (roll < 35) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Random, 
                    GetEnemyHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 35  && roll < 55) {
                spwn::enemy::GruntWithAPistol(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Random, 
                    GetEnemyHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 55 && roll < 75) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Lazy, 
                    GetEnemyHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 75 && roll < 90) {
                spwn::enemy::Grunt(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Melee, 
                    GetEnemyHp(data::cnst::GRUNT_MIN_HP, data::cnst::GRUNT_HP_MULTIPLIER)
                );
            } else if (roll >= 90 && roll < 97) {
                spwn::enemy::Brute(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Lazy, 
                    GetEnemyHp(data::cnst::BRUTE_MIN_HP, data::cnst::BRUTE_HP_MULTIPLIER)
                );
            } else {
                spwn::enemy::Brute(
                    world, 
                    Vector3{ pos.x, 0.0f, pos.z }, 
                    cmpt::AIMoveMode::Melee, 
                    GetEnemyHp(data::cnst::BRUTE_MIN_HP, data::cnst::BRUTE_HP_MULTIPLIER)
                );
            }
        }
    }
}