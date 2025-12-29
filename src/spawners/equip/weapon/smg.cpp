#include "spawners/equip/weapon/weapons.h"
#include "components/components.h"

namespace spwn::weapon {
    void EquipSMG(strg::Registry& world, const Entity id, Layer layer, Layer mask) {
        if (!world.HasComponent<cmpt::SMG>(id)) {
            world.AddComponent<cmpt::SMG>(
                id,
                cmpt::SMG{
                    .base = cmpt::WeaponBase {
                        .parent = id,
                        .kind = data::loot::WeaponKind::SMG,
                        .cooldown = data::cnst::SMG_COOLDOWN, 
                        .countdown = data::cnst::SMG_COOLDOWN,
                        .damage = data::cnst::SMG_DAMAGE,
                        .penetration = data::cnst::SMG_PENETRATION,
                        .projectile_speed = data::cnst::SMG_PROJECTILE_SPEED,
                        .knockback_scale = data::cnst::SMG_KNOCKBACK_SCALE,
                        .knockback_duration = data::cnst::SMG_KNOCKBACK_DURATION,
                        .layer = layer,
                        .mask = mask,
                        .soundfx_type = asset::SoundFxType::SMG
                    },
                }
            );
        } else {
            // TODO: upgrade random stat
            auto& wep = world.GetComponent<cmpt::SMG>(id);
            wep.base.damage += 1;
        }
    }

    void DequipSMG(strg::Registry& world, const Entity id) {
        if (world.HasComponent<cmpt::SMG>(id)) {
            world.RemoveComponent<cmpt::SMG>(id);
        }
    }
}