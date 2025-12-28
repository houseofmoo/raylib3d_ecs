
#pragma once

#include "storage/registry.h"
#include "data/game/game.h"
#include "data/entity.h"
#include "components/components.h"
#include "components/cmpt_helpers.h"
#include "spawners/system/events/notification.h"
#include "resources/asset_loader.h"
#include "utils/debug.h"

namespace sys::dmg {
    inline void ApplyDamage(strg::Registry& world) {
        PROFILE_SCOPE("ApplyDamage()");
        for (auto entity : world.View<cmpt::Stats, cmpt::DamageReceiver>()) {
            auto& stats = world.GetComponent<cmpt::Stats>(entity);
            auto& dmg = world.GetComponent<cmpt::DamageReceiver>(entity);
            if (dmg.total <= 0) {
                continue;
            }

            if (world.HasComponent<cmpt::Invulnerable>(entity)) {
                dmg.total = 0;
                continue;
            }

            // player taking damage is special
            if (world.HasComponent<cmpt::Player>(entity)) {
                // if god mode enabled, remove all player damage
                if (data::g_cheats.god_mode) {
                    dmg.total = 0;
                } 

                // add invulnerability to player
                cmpt::AttachPlayerInvulnerability(world, entity, data::cnst::INVULNRABILITY_CD);

                // pause game loop for a short time to add "impact"
                world.AddComponent<cmpt::FreezeTime>(
                    entity,
                    cmpt::FreezeTime{ .countdown = data::cnst::IMPACT_FREZE }
                );
            }
            
            world.AddComponent<cmpt::DamageFlash>(
                entity, 
                cmpt::DamageFlash{ .duration = data::cnst::DAMAGE_FLASH }
            );
            
            stats.current_hp -= dmg.total;
            dmg.total = 0;

            PlaySound(rsrc::asset::damage_fx);
            SetSoundPitch(rsrc::asset::damage_fx, (float)GetRandomValue(9, 15) * 0.1f);

            if (stats.current_hp <= 0) {
                world.AddComponent<cmpt::DeathAnimation>(
                    entity, 
                    cmpt::DeathAnimation{ .duration = 0.5f }
                );
            }
        }
    }
}