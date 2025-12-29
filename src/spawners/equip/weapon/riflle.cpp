#include "spawners/equip/weapon/weapons.h"
#include "components/components.h"

namespace spwn::weapon {
    void EquipRifle(strg::Registry& world, const Entity id, Layer layer, Layer mask) {
        // if entity does not have a shotgun, give them one
        if (!world.HasComponent<cmpt::Rifle>(id)) {
            world.AddComponent<cmpt::Rifle>(
                id,
                cmpt::Rifle{
                    .base = cmpt::WeaponBase {
                        .parent = id,
                        .kind = data::loot::WeaponKind::Rifle,
                        .cooldown = data::cnst::RIFLE_COOLDOWN, 
                        .countdown = data::cnst::RIFLE_COOLDOWN,
                        .damage = data::cnst::RIFLE_DAMAGE,
                        .penetration = data::cnst::RIFLE_PENETRATION,
                        .projectile_speed = data::cnst::RIFLE_PROJECTILE_SPEED,
                        .knockback_scale = data::cnst::RIFLE_KNOCKBACK_SCALE,
                        .knockback_duration = data::cnst::RIFLE_KNOCKBACK_DURATION,
                        .layer = layer,
                        .mask = mask,
                        .soundfx_type = asset::SoundFxType::Rifle
                    },
                    .burst_count = data::cnst::RIFLE_BURST_COUNT,
                    .burst_completed = 0,
                    .burst_cooldown = data::cnst::RIFLE_BURST_COOLDOWN,
                    .burst_countdown = data::cnst::RIFLE_BURST_COOLDOWN,
                }
            );
        } else {
            // upgrade random stat on the shotgun
            auto& wep = world.GetComponent<cmpt::Rifle>(id);
            wep.base.damage += 1;
        }
    }

    void DequipRifle(strg::Registry& world, const Entity id) {
        // if entity does not have a shotgun, give them one
        if (world.HasComponent<cmpt::Rifle>(id)) {
            world.RemoveComponent<cmpt::Rifle>(id);
        }
    }
}