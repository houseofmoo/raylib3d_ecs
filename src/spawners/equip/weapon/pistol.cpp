#include "spawners/equip/weapon/pistol.h"
#include "components/components.h"

namespace spwn::weapon {
    void EquipPistol(strg::Registry& world, const Entity id) {
        if (!world.HasComponent<cmpt::Pistol>(id)) {
            world.AddComponent<cmpt::Pistol>(
                id,
                cmpt::Pistol{
                    .base_stats = cmpt::WeaponBaseStats {
                        .parent = id,
                        .kind = data::loot::WeaponKind::Pistol,
                        .cooldown = data::cnst::PISTOL_COOLDOWN, 
                        .countdown = data::cnst::PISTOL_COOLDOWN,
                        .damage = data::cnst::PISTOL_DAMAGE,
                        .penetration = data::cnst::PISTOL_PENETRATION,
                        .projectile_speed = data::cnst::PISTOL_PROJECTILE_SPEED,
                        .knockback_scale = data::cnst::PISTOL_KNOCKBACK_SCALE,
                        .knockback_duration = data::cnst::PISTOL_KNOCKBACK_DURATION
                    },
                }
            );
        } else {
            // TODO: upgrade random stat
            auto& wep = world.GetComponent<cmpt::Pistol>(id);
            wep.base_stats.damage += 1;
        }
    }

    void DequipPistol(strg::Registry& world, const Entity id) {
        if (world.HasComponent<cmpt::Pistol>(id)) {
            world.RemoveComponent<cmpt::Pistol>(id);
        }
    }
}