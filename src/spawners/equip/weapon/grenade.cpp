#include "spawners/equip/weapon/grenade.h"
#include "components/components.h"

namespace spwn::weapon {
    void EquipGrenade(strg::Registry& world, const Entity id) {
         if (!world.HasComponent<cmpt::Grenade>(id)) {
            world.AddComponent<cmpt::Grenade>(
                id,
                cmpt::Grenade{
                    .base_stats = cmpt::WeaponBaseStats {
                        .parent = id,
                        .kind = data::loot::WeaponKind::Grenade,
                        .cooldown = data::cnst::GRENADE_COOLDOWN, 
                        .countdown = data::cnst::GRENADE_COOLDOWN,
                        .projectile_speed = data::cnst::GRENADE_PROJECTILE_SPEED,
                        .damage = data::cnst::GRENADE_DAMAGE,
                    },
                }
            );
        } else {
            // TODO: upgrade random stat
            auto& wep = world.GetComponent<cmpt::Grenade>(id);
            wep.base_stats.damage += 1;
        }
    }
    
    void DequipGrenade(strg::Registry& world, const Entity id) {
        if (world.HasComponent<cmpt::Grenade>(id)) {
           world.RemoveComponent<cmpt::Grenade>(id);
        }
    }
}