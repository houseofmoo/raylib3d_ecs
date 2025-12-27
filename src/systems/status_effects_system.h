#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "spawners/system/events/notification.h"
#include "data/entity.h"
#include "utils/debug.h"

namespace sys::se {
    inline void ApplyStatusEffects(strg::Registry& world) {
        PROFILE_SCOPE("ApplyStatusEffects()");
        // dash
        for (auto entity : world.View<cmpt::Dash, cmpt::Speed>()) {
            auto& spd = world.GetComponent<cmpt::Speed>(entity);
            auto& dash = world.GetComponent<cmpt::Dash>(entity);
            spd.dash_multiplier = dash.multiplier;
        }
    }

    inline void RemoveStatsusEffects(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("RemoveStatsusEffects()");
        // remove invulnerability
        for (auto entity : world.View<cmpt::Invulnerable>()) {
            auto& invuln = world.GetComponent<cmpt::Invulnerable>(entity);
            invuln.countdown -= delta_time;

            if (invuln.countdown <= 0.0f) {
                // restore mask
                if (auto col = world.TryGetComponent<cmpt::Collider>(entity)) {
                    col->mask = invuln.mask;
                }
                world.RemoveComponent<cmpt::Invulnerable>(entity);
                spwn::evt::Notification(world, data::notif::LOSE_INVUL);
            }
        }

        // remove knock back
        for (auto entity : world.View<cmpt::Knockback>()) {
            auto& kb = world.GetComponent<cmpt::Knockback>(entity);
            kb.countdown -= delta_time;

            if (kb.countdown <= 0.0f) {
                world.RemoveComponent<cmpt::Knockback>(entity);
            }
        }

        // remove dash exhausted
        for (auto entity : world.View<cmpt::DashExhausted>()) {
            auto& de = world.GetComponent<cmpt::DashExhausted>(entity);
            de.countdown -= delta_time;

            if (de.countdown <= 0.0f) {
                world.RemoveComponent<cmpt::DashExhausted>(entity);
            }
        }

        // remove dash
        for (auto entity : world.View<cmpt::Dash, cmpt::Speed>()) {
            auto& dash = world.GetComponent<cmpt::Dash>(entity);
            dash.countdown -= delta_time;

            if (dash.countdown <= 0.0f) {
                auto& spd = world.GetComponent<cmpt::Speed>(entity);
                spd.dash_multiplier = 1.0f;
                world.RemoveComponent<cmpt::Dash>(entity);
                world.AddComponent<cmpt::DashExhausted>(entity, cmpt::DashExhausted{1.5f});
            }
        }
    }
}