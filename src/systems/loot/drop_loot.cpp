#include "systems/loot/drop_loot.h"

#include <cinttypes>
#include "raylib.h"
#include "components/components.h"
#include "spawners/world/loot/loot.h"
#include "data/game/game.h"
#include "data/entity.h"
#include "utils/debug.h"

namespace sys::loot {
    static uint32_t enemies_since_powerup_loot_dropped = 0;
    static uint32_t enemies_since_weapon_crate_dropped = 0;

    float ColliderScaler(strg::Registry& world) {
        float collider_scale = 1.0f;
        if (auto* stats = world.TryGetComponent<cmpt::Stats>(data::g_player.id)) {
            collider_scale = stats->pickup_range_modifier;
        }
        return collider_scale;
    }

    bool ShouldDropLoot(const float drop_scaler) {
        return static_cast<int>((float)GetRandomValue(0, 99) * drop_scaler) > data::cnst::LOOT_CHANCE;
    }

    void SpawnLoot(strg::Registry& world, const Vector3 position, const float scaler) {
        int roll = GetRandomValue(0, 99);
        if (roll < 50) {
            spwn::loot::Money(world, position, data::cnst::MONEY_VALUE, scaler);
            enemies_since_powerup_loot_dropped += 1;
            enemies_since_weapon_crate_dropped += 1;
        } else if (roll >= 50 && roll < 95) {
            spwn::loot::Powerup(world, position, data::loot::GetRandomPowerupKind(), scaler);
            enemies_since_powerup_loot_dropped = 0;
        } else if (roll >= 95) {
            spwn::loot::WeaponCrate(world, position, scaler);
            enemies_since_weapon_crate_dropped = 0;
        }
    }

    void DropLoot(strg::Registry& world) {
        PROFILE_SCOPE("DropLoot()");
        if (data::g_cheats.never_drop_loot) {
            return;
        }
        
        for (auto enemy : world.View<cmpt::DropsLoot, 
                                        tag::Destroy,
                                        cmpt::Transform>()) {
            
            auto& drop = world.GetComponent<cmpt::DropsLoot>(enemy);                                   
            auto& etrans = world.GetComponent<cmpt::Transform>(enemy);
            float collider_scale = ColliderScaler(world);
  
            // all enemies that drop loot always drop exp
            spwn::loot::Exp(world, etrans.position, data::cnst::EXP_VALUE, collider_scale);

            if (data::g_cheats.always_drop_loot) {
                spwn::loot::Powerup(world, etrans.position, data::loot::GetRandomPowerupKind(), collider_scale);
                spwn::loot::WeaponCrate(world, etrans.position, collider_scale);
                continue;
            }
            
            // if N enemies die and no powerup loot has dropped, drop loot
            if (enemies_since_powerup_loot_dropped >= data::cnst::LOOT_BADLUCK_POWERUP) {
                spwn::loot::Powerup(world, etrans.position, data::loot::GetRandomPowerupKind(), collider_scale);
                enemies_since_powerup_loot_dropped = 0;
                continue;
            }

            // if N enemies die and no wep loot has dropped, drop loot
            if (enemies_since_weapon_crate_dropped >= data::cnst::LOOT_BADLUCK_WEPCRATE) {
                spwn::loot::Powerup(world, etrans.position, data::loot::GetRandomPowerupKind(), collider_scale);
                enemies_since_weapon_crate_dropped = 0;
                continue;
            }

            // check if loot roll was success
            if (!ShouldDropLoot(drop.loot_chance)) {
                enemies_since_powerup_loot_dropped += 1;
                enemies_since_weapon_crate_dropped += 1;
                continue;
            }

            SpawnLoot(world, etrans.position, collider_scale);
        }
    }
}