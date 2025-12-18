#pragma once

#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"
#include "data/player/player.h"
#include "spawners/projectile/bullet.h"
#include "components/components.h"
#include "components/tags.h"
#include "utils/rl_utils.h"

namespace sys::atk {
    inline void FireSpread(Storage::Registry& world, Vector3 origin, Vector3 baseDir, int pelletCount,
                    float totalSpreadDegrees, const int damage, const int penetration) {
        
        // Convert to radians
        float totalSpread = totalSpreadDegrees * (PI / 180.0f);
        float half = totalSpread * 0.5f;

        for (int i = 0; i < pelletCount; i++) {
            float t = (float)i / (float)(pelletCount - 1);     // 0..1
            float angle = Lerp(-half, +half, t);               // -half..+half
            Vector3 dir = utils::RotateDirYaw(baseDir, angle);
            //dir = Vector3Scale(Vector3Normalize(dir), speed);
            //dir = Vector3Scale(dir, speed);

            spwn::proj::Bullet(
                world, 
                origin, 
                dir, 
                damage,
                penetration
            );
        }
    }

    inline void WeaponAttacks(Storage::Registry& world, const float delta_time, Sound& sound_fx) {
        for (auto weapon : world.View<tag::Weapon, 
                                    cmpt::WeaponStats>()) {
            
            auto& stats = world.GetComponent<cmpt::WeaponStats>(weapon);
            stats.countdown -= delta_time;
            if (stats.countdown > 0.0f) continue;

            stats.countdown = stats.cooldown / data::player::g_player.attack_speed_multiplier;
            if (stats.countdown <= 0.0f) stats.countdown = 0.1f;
                        
            // get parent transform
            auto parent_trans = world.TryGetComponent<cmpt::Transform>(stats.parent);
            if (parent_trans == nullptr) { 
                world.AddComponent<tag::Destroy>(weapon);
                continue;
            }

            // get parent mouse world position
            auto parent_input = world.TryGetComponent<cmpt::Input>(stats.parent);
            if (parent_input == nullptr) { 
                world.AddComponent<tag::Destroy>(weapon);
                continue;
            }

            Vector3 direction = Vector3Subtract(parent_input->mouse_world_position, parent_trans->position);
            if (Vector3LengthSqr(direction) < 0.000001f) continue;
            direction = Vector3Normalize(direction);
            direction = Vector3 { direction.x * stats.projectile_speed, 0.0f, direction.z * stats.projectile_speed };

            // if (stats.pellet_count <= 1) {
                spwn::proj::Bullet(
                    world, 
                    parent_trans->position, 
                    direction,  
                    stats.damage * data::player::g_player.damage_multiplier,
                    1//stats.penetration
                );
            // } else {
            //     float spread_deg = 20.0f;
            //     FireSpread(
            //         world, 
            //         parent_trans->position, 
            //         direction, 
            //         stats.pellet_count, 
            //         spread_deg, 
            //         stats.damage * data::player::g_player.damage_multiplier,
            //         1//stats.penetration
            //     );
            // }
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
            //Spawner::Bullet(world, parent_trans->position, direction, stats.pellet_speed, stats.damage);


            // test
            // spwn::bullet::Grenade(
            //     world,
            //     parent_trans->position, 
            //     {direction.x, 25.0f, direction.z},
            //     parent_input->mouse_world_position,
            //     &rsrc::asset::bullet_model, 
            //     stats.damage * data::player::g_player.damage_multiplier
            // );
        }
    }
}