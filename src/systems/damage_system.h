
#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "spawners/system/events/notification.h"
#include "resources/asset_loader.h"
#include "utils/debug.h"

namespace sys::dmg {
    inline void ApplyDamage(Storage::Registry& world) {
        PROFILE_SCOPE("ApplyDamage()");
        for (auto entity : world.View<cmpt::Health, cmpt::DamageReceiver>()) {
            auto& hp = world.GetComponent<cmpt::Health>(entity);
            auto& dmg = world.GetComponent<cmpt::DamageReceiver>(entity);
            if (dmg.total <= 0) {
                continue;
            }

            if (world.HasComponent<cmpt::Invulnerable>(entity)) {
                dmg.total = 0;
                continue;
            }

            if (world.HasComponent<tag::Player>(entity)) {
                // if god mode enabled, remove all player damage
                if (data::player::g_player.god_mode) {
                    dmg.total = 0;
                } 

                auto& col = world.GetComponent<cmpt::Collider>(entity);
                world.AddComponent<cmpt::Invulnerable>(
                    entity, 
                    cmpt::Invulnerable{ .mask = col.mask, .countdown = 1.0f}
                );
                
                // while invulnerable do not interact with enemies
                col.mask = col.mask & ~data::layer::ENEMY;

                spwn::evt::Notification(
                    world, 
                    std::string("- " + std::to_string(dmg.total) + " HP, GAIN INVUL")
                );
            }
            
            world.AddComponent<cmpt::DamageFlash>(
                entity, 
                cmpt::DamageFlash{ .duration = 0.1f }
            );
            PRINT("dmg received: ", dmg.total);
            hp.amount -= dmg.total;
            dmg.total = 0;
            PlaySound(rsrc::asset::damage_fx);
            SetSoundPitch(rsrc::asset::damage_fx, (float)GetRandomValue(9, 15) * 0.1f);

            if (hp.amount <= 0) {
                world.AddComponent<cmpt::DeathAnimation>(
                    entity, 
                    cmpt::DeathAnimation{ .duration = 0.5f }
                );
                //world.AddComponent<tag::Destroy>(entity);
            }
        }
    }
}