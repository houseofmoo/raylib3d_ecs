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
        strg::Registry& world,
        int pellet_count,
        float spread_degrees,
        spwn::proj::BulletConfig config) {

        // save original directino for angle calcs
        Vector3 base_dir = config.direction;

        // convert to radians
        float spread_rad = spread_degrees * (PI / 180.0f);
        float half = spread_rad * 0.5f;

        for (int i = 0; i < pellet_count; i++) {
            float t = (float)i / (float)(pellet_count - 1);
            float angle = Lerp(-half, +half, t);
            config.direction = utils::RotateDirYaw(base_dir, angle);
            spwn::proj::Bullet(world, config);
        }
    }

    void PistolAttack(strg::Registry& world, const float delta_time, Sound& sound_fx) {
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
                    .penetration = wep.base_stats.penetration,
                    .knockback_scale = wep.base_stats.knockback_scale,
                    .knockback_duration = wep.base_stats.knockback_duration
                }
            );
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
        }
    }

    void ShotgunAttack(strg::Registry& world, const float delta_time, Sound& sound_fx) {
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
                    .penetration = wep.base_stats.penetration,
                    .knockback_scale = wep.base_stats.knockback_scale,
                    .knockback_duration = wep.base_stats.knockback_duration
                }
            );
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
        }
    }

    void RifleAttack(strg::Registry& world, const float delta_time, Sound& sound_fx) {
        PROFILE_SCOPE("RifleAttack()");
        for (auto entity : world.View<cmpt::Rifle>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& input = world.GetComponent<cmpt::Input>(entity);
            auto& wep = world.GetComponent<cmpt::Rifle>(entity);
     
            // if doing a burst
            if (wep.burst_completed < wep.burst_count) {
                // check if burst is ready
                wep.burst_countdown -= delta_time;
                if (wep.burst_countdown > 0.0f) continue;
                
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
                        .penetration = wep.base_stats.penetration,
                        .knockback_scale = wep.base_stats.knockback_scale,
                        .knockback_duration = wep.base_stats.knockback_duration
                    }
                );
                PlaySound(sound_fx);
                SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);

                wep.burst_completed += 1;
                wep.burst_countdown = wep.burst_cooldown;
            } else {
                // check if ready to fire
                wep.base_stats.countdown -= delta_time;
                if (wep.base_stats.countdown > 0.0f) continue;

                wep.base_stats.countdown = wep.base_stats.cooldown / data::g_player.attack_speed_multiplier;
                if (wep.base_stats.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                    wep.base_stats.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
                }

                wep.burst_completed = 0;
                wep.burst_countdown = 0.0f;
            }
        }
    }

    void GrenadeLauncherAttack(strg::Registry& world, const float delta_time, Sound& sound_fx) {
        PROFILE_SCOPE("GrenadeAttack()");
        for (auto entity : world.View<cmpt::GrenadeLauncher>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);
            auto& input = world.GetComponent<cmpt::Input>(entity);
            auto& wep = world.GetComponent<cmpt::GrenadeLauncher>(entity);
     
            wep.base_stats.countdown -= delta_time;
            if (wep.base_stats.countdown > 0.0f) continue;

            wep.base_stats.countdown = wep.base_stats.cooldown / data::g_player.attack_speed_multiplier;
            if (wep.base_stats.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base_stats.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            } 

            spwn::proj::Grenade(
                world, 
                trans.position, 
                input.mouse_world_position,
                spwn::proj::GrenadeConfig {
                    .damage = static_cast<int>(wep.base_stats.damage * data::g_player.damage_multiplier),
                    .penetration = wep.base_stats.penetration,
                    .knockback_scale = wep.base_stats.knockback_scale,
                    .knockback_duration = wep.base_stats.knockback_duration,
                    
                    .arch_duration = wep.arch_duration,
                    .arch_max_height = wep.arch_max_height,

                    .explosion_damage = static_cast<int>(wep.explosion_damage * data::g_player.damage_multiplier),
                    .explosion_start_size = wep.explosion_start_size,
                    .explosion_end_size = wep.explosion_end_size,
                    .explosion_duration = wep.explosion_duration,
                    .explosion_knockback_scale = wep.explosion_knockback_scale,
                    .explosion_knockback_duration = wep.explosion_knockback_duration
                }
            );
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
        }
    }

    void ExplosionAttack(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ApplyExpands()");
        for (auto entity : world.View<cmpt::Explosion, cmpt::Collider, cmpt::Draw>()) {

            auto& exp = world.GetComponent<cmpt::Explosion>(entity);
            auto& col = world.GetComponent<cmpt::Collider>(entity);
            auto& draw = world.GetComponent<cmpt::Draw>(entity);

            exp.elapsed += delta_time;
            float t = Clamp(exp.elapsed / exp.duration, 0.0f, 1.0f);
  
            // we want the collider to be a bit larger than the
            // draw animations to make it feel better

            // collider grows larger over time
            col.size = Vector3Lerp(exp.start_size, exp.end_size, t);
            draw.size = Vector3Scale(col.size, 0.9); // for wires

            // base model size.x is 0.5f
            float scale = (col.size.x / data::cnst::GRENADE_SIZE.x) * 0.9f;
            draw.scale = { scale, scale, scale };
        }
    }

    void WeaponAttacks(strg::Registry& world, const float delta_time, Sound& sound_fx) {
        PistolAttack(world, delta_time, sound_fx);
        ShotgunAttack(world, delta_time, sound_fx);
        RifleAttack(world, delta_time, sound_fx);
        GrenadeLauncherAttack(world, delta_time, sound_fx);
        ExplosionAttack(world, delta_time);
    }
}