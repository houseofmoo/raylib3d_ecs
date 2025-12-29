#include "spawners/equip/weapon/weapons.h"
#include "components/components.h"

namespace spwn::weapon {
    void EquipShotgun(strg::Registry& world, const Entity id, Layer layer, Layer mask) {
        // if entity does not have a shotgun, give them one
        if (!world.HasComponent<cmpt::Shotgun>(id)) {
            world.AddComponent<cmpt::Shotgun>(
                id,
                cmpt::Shotgun{
                    .base = cmpt::WeaponBase {
                        .parent = id,
                        .kind = data::loot::WeaponKind::Shotgun,
                        .cooldown = data::cnst::SHOTGUN_COOLDOWN, 
                        .countdown = data::cnst::SHOTGUN_COOLDOWN,
                        .damage = data::cnst::SHOTGUN_DAMAGE,
                        .penetration = data::cnst::SHOTGUN_PENETRATION,
                        .projectile_speed = data::cnst::SHOTGUN_PROJECTILE_SPEED,
                        .knockback_scale = data::cnst::SHOTGUN_KNOCKBACK_SCALE,
                        .knockback_duration = data::cnst::SHOTGUN_KNOCKBACK_DURATION,
                        .layer = layer,
                        .mask = mask,
                        .soundfx_type = asset::SoundFxType::Shotgun
                    },
                    .spread_deg = data::cnst::SHOTGUN_SPREAD,
                    .pellet_count = data::cnst::SHOTGUN_PELLET_COUNT
                }
            );
        } else {
            // upgrade random stat on the shotgun
            auto& wep = world.GetComponent<cmpt::Shotgun>(id);
            wep.base.damage += 1;
        }
    }

    void DequipShotgun(strg::Registry& world, const Entity id) {
        // if entity does not have a shotgun, give them one
        if (world.HasComponent<cmpt::Shotgun>(id)) {
            world.RemoveComponent<cmpt::Shotgun>(id);
        }
    }
}