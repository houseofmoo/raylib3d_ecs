
#include "systems/enemy/timed_spawn.h"

#include "data/game/game.h"
#include "spawners/enemy/enemy.h"

namespace sys::enemy {
    float spawn_countdown = data::game::g_enemy_spawn_interval;

    void SpawnEnemyInterval(Storage::Registry& world, const BoundingBox& boundary, const float delta_time) {
        spawn_countdown -= delta_time;
        if (spawn_countdown > 0.0f) return;
        
        
        data::game::g_difficulty_level += 1;

        // increase enemy hp based on difficulty
        int enemy_hp = (data::game::g_difficulty_level / 8) + 25;
        
        // from difficulty 0..100 spawn enemies faster
        data::game::g_enemy_spawn_interval = 1.5f - (data::game::g_difficulty_level * 0.01f);
        if (data::game::g_enemy_spawn_interval < 0.5f) {
            data::game::g_enemy_spawn_interval = 0.5f;
        }
        spawn_countdown = data::game::g_enemy_spawn_interval;

        // after difficulty 100, start spawning multiple enemies
        int enemy_count = (data::game::g_difficulty_level / 100) + 1;

        for (int i = 0; i < enemy_count; i++) {
            float enemy_x = (float)GetRandomValue(boundary.min.x + 1.0f, boundary.max.x - 1.0f);
            float enemy_z = (float)GetRandomValue(boundary.min.z + 1.0f, boundary.max.z - 1.0f);
            spwn::enemy::Enemy(
                world, 
                Vector3{ enemy_x, 0.0f, enemy_z }, 
                enemy_hp
            );
        }
    }
}