#include "systems/attack/attack_system.h"
#include "raymath.h"
#include "data/player/player.h"
#include "data/entity.h"
#include "spawners/world/projectile/bullet.h"
#include "spawners/world/projectile/grenade.h"
#include "components/components.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::atk {
    inline void FireSpread(
        Storage::Registry& world,
        int pellet_count,
        float spread_degrees,
        spwn::proj::BulletConfig config) {
            
        Vector3 base_dir = config.direction;

        // Convert to radians
        float spread_rad = spread_degrees * (PI / 180.0f);
        float half = spread_rad * 0.5f;

        for (int i = 0; i < pellet_count; i++) {
            float t = (float)i / (float)(pellet_count - 1);
            float angle = Lerp(-half, +half, t);
            Vector3 dir = utils::RotateDirYaw(base_dir, angle);
            config.direction = dir;
            spwn::proj::Bullet(world, config);
        }
    }

    // inline void FireSpread(
    //     Storage::Registry& world, 
    //     Vector3 origin, 
    //     Vector3 base_dir, 
    //     int pellet_count,
    //     float totalSpreadDegrees, 
    //     const int damage, 
    //     const int penetration) {
        
    //     // Convert to radians
    //     float spread_rad = totalSpreadDegrees * (PI / 180.0f);
    //     float half = spread_rad * 0.5f;

    //     for (int i = 0; i < pellet_count; i++) {
    //         float t = (float)i / (float)(pellet_count - 1);
    //         float angle = Lerp(-half, +half, t);
    //         Vector3 dir = utils::RotateDirYaw(base_dir, angle);
    //         spwn::proj::Bullet(
    //             world, 
    //             origin,
    //             dir, 
    //             damage,
    //             penetration
    //         );
    //     }
    // }

    void PistolAttack(Storage::Registry& world, const float delta_time, Sound& sound_fx) {
        PROFILE_SCOPE("PistolAttack()");
        for (auto entity : world.View<cmpt::Pistol>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& input = world.GetComponent<cmpt::Input>(entity);
            auto& wep = world.GetComponent<cmpt::Pistol>(entity);
     
            wep.base_stats.countdown -= delta_time;
            if (wep.base_stats.countdown > 0.0f) continue;

            wep.base_stats.countdown = wep.base_stats.cooldown / data::g_player.attack_speed_multiplier;
            if (wep.base_stats.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base_stats.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            } 

            Vector3 direction = utils::DirectionFlattenThenNormalize(
                trans.position, 
                input.mouse_world_position
            );

            direction = Vector3Scale(direction, wep.base_stats.projectile_speed);
            spwn::proj::Bullet(
                world,
                spwn::proj::BulletConfig{
                    .position = trans.position,
                    .direction = direction,
                    .damage = (int)(wep.base_stats.damage * data::g_player.damage_multiplier),
                    .penetration = data::cnst::PISTOL_PENETRATION,
                    .knockback_scale = data::cnst::PISTOL_KNOCKBACK_SCALE,
                    .knockback_duration = data::cnst::PISTOL_KNOCKBACK_DURATION
                }
            );
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
        }
    }

    void ShotgunAttack(Storage::Registry& world, const float delta_time, Sound& sound_fx) {
        PROFILE_SCOPE("ShotgunAttack()");
        for (auto entity : world.View<cmpt::Shotgun>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& input = world.GetComponent<cmpt::Input>(entity);
            auto& wep = world.GetComponent<cmpt::Shotgun>(entity);
     
            wep.base_stats.countdown -= delta_time;
            if (wep.base_stats.countdown > 0.0f) continue;

            wep.base_stats.countdown = wep.base_stats.cooldown / data::g_player.attack_speed_multiplier;
            if (wep.base_stats.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base_stats.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            } 

            Vector3 direction = Vector3Scale(
                utils::DirectionFlattenThenNormalize(
                    trans.position, 
                    input.mouse_world_position
                ),
                wep.base_stats.projectile_speed
            );

            FireSpread(
                world, 
                wep.pellet_count, 
                wep.spread_deg,
                spwn::proj::BulletConfig{
                    .position = trans.position,
                    .direction = direction,
                    .damage = (int)(wep.base_stats.damage * data::g_player.damage_multiplier),
                    .penetration = data::cnst::SHOTGUNL_PENETRATION,
                    .knockback_scale = data::cnst::SHOTGUN_KNOCKBACK_SCALE,
                    .knockback_duration = data::cnst::SHOTGUN_KNOCKBACK_DURATION
                }
            );
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
        }
    }

    void GrenadeAttack(Storage::Registry& world, const float delta_time, Sound& sound_fx) {
        PROFILE_SCOPE("GrenadeAttack()");
        for (auto entity : world.View<cmpt::Grenade>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& input = world.GetComponent<cmpt::Input>(entity);
            auto& wep = world.GetComponent<cmpt::Grenade>(entity);
     
            wep.base_stats.countdown -= delta_time;
            if (wep.base_stats.countdown > 0.0f) continue;

            wep.base_stats.countdown = wep.base_stats.cooldown / data::g_player.attack_speed_multiplier;
            if (wep.base_stats.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base_stats.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            } 

            float dmg = wep.base_stats.damage * data::g_player.damage_multiplier;
            spwn::proj::Grenade(
                world, 
                trans.position, 
                input.mouse_world_position,
                dmg
            );
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
        }
    }

    void WeaponAttacks(Storage::Registry& world, const float delta_time, Sound& sound_fx) {
        PistolAttack(world, delta_time, sound_fx);
        ShotgunAttack(world, delta_time, sound_fx);
        GrenadeAttack(world, delta_time, sound_fx);
    }
}