#pragma once

#include "storage/registry.h"
#include "spawners/world/projectile/explosion.h"
#include "components/components.h"
#include "data/player/player.h"
#include "data/game/game.h"
#include "utils/debug.h"

namespace sys::cleanup {
    void Cleanup(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("Cleanup()");
        for (auto entity : world.View<cmpt::Lifetime>()) {
            auto& lifetime = world.GetComponent<cmpt::Lifetime>(entity);
            lifetime.countdown -= delta_time;

            if (lifetime.countdown <= 0.0f) {
                world.AddComponent<tag::Destroy>(entity);
                // world.DestroyEntity(entity);
            }
        }
    }

    void OnDestroy(strg::Registry& world) {
        PROFILE_SCOPE("Destroy()");
        for (auto entity : world.View<tag::Destroy,
                                      cmpt::Transform>()) {

            // check for on destroy effects
            if (world.HasComponent<cmpt::ExplodeOnDestroy>(entity)) {
                auto& exp = world.GetComponent<cmpt::ExplodeOnDestroy>(entity);
                auto& trans = world.GetComponent<cmpt::Transform>(entity);
                spwn::proj::Explosion(
                    world,
                    spwn::proj::ExplosionConfig {
                        .position = trans.position,
                        .start_size = exp.start_size,
                        .end_size = exp.end_size,
                        .damage = static_cast<int>(exp.damage * data::g_player.damage_multiplier),
                        .duration = exp.duration,
                        .knockback_scale = exp.knockback_scale,
                        .knockback_duration = exp.knockback_duration

                    }
                );
            }
        }
    }

    void Destroy(strg::Registry& world) {
        PROFILE_SCOPE("Destroy()");
        for (auto entity : world.View<tag::Destroy>()) {
            // if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
            //     auto& death_anim = world.GetComponent<cmpt::DeathAnimation>(entity);
            //     death_anim.elapsed += delta_time;
            //     if (death_anim.elapsed < death_anim.duration) {
            //         continue;
            //     }
            //     // world.RemoveComponent<cmpt::DeathAnimation>(entity);
            //     // world.AddComponent<tag::Destroy>(entity);
            // }
            if (world.HasComponent<tag::Enemy>(entity)) {
                data::g_player.enemies_defeated += 1;
            }

            if (world.HasComponent<tag::Player>(entity)) {
                data::g_game.state = data::GameState_E::Dead;
            }

            world.DestroyEntity(entity);
        }
    }
}