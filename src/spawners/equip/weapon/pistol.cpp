#include "spawners/equip/weapon/weapons.h"
#include "components/components.h"
#include "assets/assets.h"

namespace spwn::weapon {
    void EquipPistol(strg::Registry& world, const Entity id, Layer layer, Layer mask) {
        if (!world.HasComponent<cmpt::Pistol>(id)) {
            world.AddComponent<cmpt::Pistol>(
                id,
                cmpt::Pistol{
                    .base = cmpt::WeaponBase {
                        .parent = id,
                        .kind = data::loot::WeaponKind::Pistol,
                        .cooldown = data::cnst::PISTOL_COOLDOWN, 
                        .countdown = data::cnst::PISTOL_COOLDOWN,
                        .damage = data::cnst::PISTOL_DAMAGE,
                        .penetration = data::cnst::PISTOL_PENETRATION,
                        .projectile_speed = data::cnst::PISTOL_PROJECTILE_SPEED,
                        .knockback_scale = data::cnst::PISTOL_KNOCKBACK_SCALE,
                        .knockback_duration = data::cnst::PISTOL_KNOCKBACK_DURATION,
                        .layer = layer,
                        .mask = mask,
                        .soundfx_type = asset::SoundFxType::Pistol
                    },
                }
            );
        } else {
            // TODO: upgrade random stat
            auto& wep = world.GetComponent<cmpt::Pistol>(id);
            wep.base.damage += 1;
        }
    }

    void DequipPistol(strg::Registry& world, const Entity id) {
        if (world.HasComponent<cmpt::Pistol>(id)) {
            world.RemoveComponent<cmpt::Pistol>(id);
        }
    }
}