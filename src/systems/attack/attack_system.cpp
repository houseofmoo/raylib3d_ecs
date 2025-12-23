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
            float dmg = wep.base_stats.damage * data::g_player.damage_multiplier;
            spwn::proj::Bullet(world, trans.position, direction, dmg, 1);
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

            Vector3 direction = utils::DirectionFlattenThenNormalize(
                trans.position, 
                input.mouse_world_position
            );

            direction = Vector3Scale(direction, wep.base_stats.projectile_speed);
            float dmg = wep.base_stats.damage * data::g_player.damage_multiplier;
            FireSpread(world, trans.position, direction, wep.pellet_count, wep.spread_deg, dmg, 1);
            PlaySound(sound_fx);
            SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);
        }
    }

    void GrenadeAttack(Storage::Registry& world, const float delta_time, Sound& sound_fx) {
        PROFILE_SCOPE("ShotgunAttack()");
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