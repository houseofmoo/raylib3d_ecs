#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "spawners/events/notification.h"
#include "utils/debug.h"

namespace sys::se {
    inline void ApplyStatusEffects(Storage::Registry& world) {
        PROFILE_SCOPE("ApplyStatusEffects()");
        // dash
        for (auto entity : world.View<cmpt::Dash, cmpt::Speed>()) {
            auto& spd = world.GetComponent<cmpt::Speed>(entity);
            auto& dash = world.GetComponent<cmpt::Dash>(entity);
            spd.dash_multiplier = dash.multiplier;
        }
    }

    inline void RemoveStatsusEffects(Storage::Registry& world) {
        PROFILE_SCOPE("RemoveStatsusEffects()");
        // remove invulnerability
        for (auto entity : world.View<cmpt::Invulnerable>()) {
            auto& invuln = world.GetComponent<cmpt::Invulnerable>(entity);
            invuln.countdown -= GetFrameTime();

            if (invuln.countdown <= 0.0f) {
                // restore mask
                if (auto col = world.TryGetComponent<cmpt::Collider>(entity)) {
                    col->mask = invuln.mask;
                }
                world.RemoveComponent<cmpt::Invulnerable>(entity);
                
                spwn::evt::Notification(
                    world, 
                    std::string("-INVUL")
                );
            }
        }

        // remove knock back
        for (auto entity : world.View<cmpt::Knockback>()) {
            auto& kb = world.GetComponent<cmpt::Knockback>(entity);
            kb.countdown -= GetFrameTime();

            if (kb.countdown <= 0.0f) {
                world.RemoveComponent<cmpt::Knockback>(entity);
            }
        }

        // remove dash exhausted
        for (auto entity : world.View<cmpt::DashExhausted>()) {
            auto& de = world.GetComponent<cmpt::DashExhausted>(entity);
            de.countdown -= GetFrameTime();

            if (de.countdown <= 0.0f) {
                world.RemoveComponent<cmpt::DashExhausted>(entity);
            }
        }

        // remove dash
        for (auto entity : world.View<cmpt::Dash, cmpt::Speed>()) {
            auto& dash = world.GetComponent<cmpt::Dash>(entity);
            dash.countdown -= GetFrameTime();

            if (dash.countdown <= 0.0f) {
                auto& spd = world.GetComponent<cmpt::Speed>(entity);
                spd.dash_multiplier = 1.0f;
                world.RemoveComponent<cmpt::Dash>(entity);
                world.AddComponent<cmpt::DashExhausted>(entity, cmpt::DashExhausted{1.5f});
            }
        }
    }
}