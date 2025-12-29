#include "systems/attack/attack_system.h"
#include "raymath.h"
#include "assets/assets.h"
#include "sound/sound_player.h"
#include "data/entity.h"
#include "spawners/world/projectile/bullet.h"
#include "spawners/world/projectile/grenade.h"
#include "components/components.h"
#include "utils/rl_utils.h"
#include "utils/debug.h"

namespace sys::atk {
    void FireSpread(
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

    void PistolAttack(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("PistolAttack()");
        for (auto entity : world.View<cmpt::Pistol, cmpt::AttackIntent, cmpt::Stats>()) {
            auto& atk = world.GetComponent<cmpt::AttackIntent>(entity);
            if (!atk.active) continue;

            auto& wep = world.GetComponent<cmpt::Pistol>(entity);
            wep.base.countdown -= delta_time;
            if (wep.base.countdown > 0.0f) continue;

            auto& stats = world.GetComponent<cmpt::Stats>(entity);
            wep.base.countdown = wep.base.cooldown / stats.attack_speed_modifier;
            if (wep.base.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            }

            spwn::proj::Bullet(
                world, 
                spwn::proj::BulletConfig{
                    .position = atk.from_position,
                    .direction = atk.direction * wep.base.projectile_speed,
                    .damage = static_cast<int>(wep.base.damage * stats.damage_modifier),
                    .penetration = wep.base.penetration,
                    .knockback_scale = wep.base.knockback_scale,
                    .knockback_duration = wep.base.knockback_duration,
                    .layer = wep.base.layer,
                    .mask = wep.base.mask
                }
            );
            snd::PlaySoundFxPositional(wep.base.soundfx_type, atk.from_position);
        }
    }

    void ShotgunAttack(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ShotgunAttack()");
        for (auto entity : world.View<cmpt::Shotgun, cmpt::AttackIntent, cmpt::Stats>()) {
            auto& atk = world.GetComponent<cmpt::AttackIntent>(entity);
            if (!atk.active) continue;

            auto& wep = world.GetComponent<cmpt::Shotgun>(entity);
            wep.base.countdown -= delta_time;
            if (wep.base.countdown > 0.0f) continue;

            auto& stats = world.GetComponent<cmpt::Stats>(entity);
            wep.base.countdown = wep.base.cooldown / stats.attack_speed_modifier;
            if (wep.base.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            }

            FireSpread(
                world, 
                wep.pellet_count, 
                wep.spread_deg,
                spwn::proj::BulletConfig{
                    .position = atk.from_position,
                    .direction = atk.direction * wep.base.projectile_speed,
                    .damage = static_cast<int>(wep.base.damage * stats.damage_modifier),
                    .penetration = wep.base.penetration,
                    .knockback_scale = wep.base.knockback_scale,
                    .knockback_duration = wep.base.knockback_duration,
                    .layer = wep.base.layer,
                    .mask = wep.base.mask
                }
            );
            snd::PlaySoundFxPositional(wep.base.soundfx_type, atk.from_position);
        }
    }

    void RifleAttack(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("RifleAttack()");
        for (auto entity : world.View<cmpt::Rifle, cmpt::AttackIntent, cmpt::Stats>()) {
            auto& atk = world.GetComponent<cmpt::AttackIntent>(entity);
            if (!atk.active) continue;

            auto& wep = world.GetComponent<cmpt::Rifle>(entity);
            auto& stats = world.GetComponent<cmpt::Stats>(entity);
            if (wep.burst_completed < wep.burst_count) {
                // check if burst is ready            
                
                wep.burst_countdown -= delta_time;
                if (wep.burst_countdown > 0.0f) continue;
                
                spwn::proj::Bullet(
                    world,
                    spwn::proj::BulletConfig{
                        .position = atk.from_position,
                        .direction = atk.direction * wep.base.projectile_speed,
                        .damage = static_cast<int>(wep.base.damage * stats.damage_modifier),
                        .penetration = wep.base.penetration,
                        .knockback_scale = wep.base.knockback_scale,
                        .knockback_duration = wep.base.knockback_duration,
                        .layer = wep.base.layer,
                        .mask = wep.base.mask
                    }
                );
                snd::PlaySoundFxPositional(wep.base.soundfx_type, atk.from_position);

                wep.burst_completed += 1;
                wep.burst_countdown = wep.burst_cooldown;
            } else {
                wep.base.countdown -= delta_time;
                if (wep.base.countdown > 0.0f) continue;

                wep.base.countdown = wep.base.cooldown / stats.attack_speed_modifier;
                if (wep.base.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                    wep.base.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
                }

                wep.burst_completed = 0;
                wep.burst_countdown = 0.0f;
            }
        }
    }

    void SMGAttack(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("SMGAttack()");
        for (auto entity : world.View<cmpt::SMG, cmpt::AttackIntent, cmpt::Stats>()) {
            auto& atk = world.GetComponent<cmpt::AttackIntent>(entity);
            if (!atk.active) continue;

            auto& wep = world.GetComponent<cmpt::SMG>(entity);
            wep.base.countdown -= delta_time;
            if (wep.base.countdown > 0.0f) continue;

            auto& stats = world.GetComponent<cmpt::Stats>(entity);
            wep.base.countdown = wep.base.cooldown / stats.attack_speed_modifier;
            if (wep.base.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            }

            spwn::proj::Bullet(
                world, 
                spwn::proj::BulletConfig{
                    .position = atk.from_position,
                    .direction = atk.direction * wep.base.projectile_speed,
                    .damage = static_cast<int>(wep.base.damage * stats.damage_modifier),
                    .penetration = wep.base.penetration,
                    .knockback_scale = wep.base.knockback_scale,
                    .knockback_duration = wep.base.knockback_duration,
                    .layer = wep.base.layer,
                    .mask = wep.base.mask
                }
            );
            snd::PlaySoundFxPositional(wep.base.soundfx_type, atk.from_position);
        }
    }

    void GrenadeLauncherAttack(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("GrenadeAttack()");
        for (auto entity : world.View<cmpt::GrenadeLauncher, cmpt::AttackIntent, cmpt::Stats>()) {
              auto& atk = world.GetComponent<cmpt::AttackIntent>(entity);
            if (!atk.active) continue;

            auto& wep = world.GetComponent<cmpt::GrenadeLauncher>(entity);
            wep.base.countdown -= delta_time;
            if (wep.base.countdown > 0.0f) continue;

            auto& stats = world.GetComponent<cmpt::Stats>(entity);
            wep.base.countdown = wep.base.cooldown / stats.attack_speed_modifier;
            if (wep.base.countdown < data::cnst::MIN_WEAPON_COOLDOWN) {
                wep.base.countdown = data::cnst::MIN_WEAPON_COOLDOWN;
            }

            spwn::proj::Grenade(
                world, 
                atk.from_position, 
                atk.to_position,
                spwn::proj::GrenadeConfig {
                    .damage = static_cast<int>(wep.base.damage * stats.damage_modifier),
                    .penetration = wep.base.penetration,
                    .knockback_scale = wep.base.knockback_scale,
                    .knockback_duration = wep.base.knockback_duration,
                    .arch_duration = wep.arch_duration,
                    .arch_max_height = wep.arch_max_height,
                    .explosion_damage = static_cast<int>(wep.explosion_damage *  stats.damage_modifier),
                    .explosion_start_size = wep.explosion_start_size,
                    .explosion_end_size = wep.explosion_end_size,
                    .explosion_duration = wep.explosion_duration,
                    .explosion_knockback_scale = wep.explosion_knockback_scale,
                    .explosion_knockback_duration = wep.explosion_knockback_duration,
                    .layer = wep.base.layer,
                    .mask = wep.base.mask
                }
            );
            snd::PlaySoundFxPositional(wep.base.soundfx_type, atk.from_position); 
        }
    }

    void ExplosionAttack(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("ExplosionAttack()");
        for (auto entity : world.View<cmpt::Explosion, cmpt::Collider, cmpt::Draw>()) {

            auto& exp = world.GetComponent<cmpt::Explosion>(entity);
            auto& col = world.GetComponent<cmpt::Collider>(entity);
            auto& draw = world.GetComponent<cmpt::Draw>(entity);

            exp.elapsed += delta_time;
            float t = Clamp(exp.elapsed / exp.duration, 0.0f, 1.0f);

            // collider grows larger over time, draw is smaller than collider
            // to make it feel more "fair"
            col.size = Vector3Lerp(exp.start_size, exp.end_size, t);
            draw.size = col.size * 0.9f; // for wires

            // base model size.x is 0.5f
            float scale = (col.size.x / data::cnst::GRENADE_SIZE.x) * 0.9f;
            draw.scale = { scale, scale, scale };
        }
    }

    // void DamagePulse(strg::Registry& world, const float delta_time) {
    //     // this is damage that pulses out from the player
    // }

    void WeaponAttacks(strg::Registry& world, const float delta_time) {
        PistolAttack(world, delta_time);
        ShotgunAttack(world, delta_time);
        RifleAttack(world, delta_time);
        SMGAttack(world, delta_time);
        GrenadeLauncherAttack(world, delta_time);
        ExplosionAttack(world, delta_time);
    }
}