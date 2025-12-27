#include "spawners/equip/weapon/weapons.h"
#include "components/components.h"

namespace spwn::weapon {
    void EquipGrenadeLauncher(strg::Registry& world, const Entity id) {
         if (!world.HasComponent<cmpt::GrenadeLauncher>(id)) {
            world.AddComponent<cmpt::GrenadeLauncher>(
                id,
                cmpt::GrenadeLauncher{
                    .base_stats = cmpt::WeaponBaseStats {
                        .parent = id,
                        .kind = data::loot::WeaponKind::GrenadeLauncher,
                        .cooldown = data::cnst::GRENADE_COOLDOWN, 
                        .countdown = data::cnst::GRENADE_COOLDOWN,
                        .damage = data::cnst::GRENADE_DAMAGE,
                        .penetration = data::cnst::GRENADE_PENETRATION,
                        .projectile_speed = data::cnst::GRENADE_PROJECTILE_SPEED,
                        .knockback_scale = data::cnst::GRENADE_KNOCKBACK_SCALE,
                        .knockback_duration = data::cnst::GRENADE_KNOCKBACK_DURATION
                    },
                    .arch_duration = data::cnst::GRENADE_ARCH_DURATION,
                    .arch_max_height = data::cnst::GRENADE_ARCH_MAX_HEIGHT,
                    .explosion_damage = data::cnst::GRENADE_EXPLOSION_DAMAGE,
                    .explosion_start_size = data::cnst::GRENADE_EXPLOSION_START_SIZE,
                    .explosion_end_size = data::cnst::GRENADE_EXPLOSION_END_SIZE,
                    .explosion_duration = data::cnst::GRENADE_EXPLOSION_DURATION,
                    .explosion_knockback_scale = data::cnst::GRENADE_EXPLOSION_KNOCKBACK_SCALE,
                    .explosion_knockback_duration = data::cnst::GRENADE_EXPLOSION_KNOCKBACK_DURATION
                }
            );
        } else {
            // TODO: upgrade random stat
            auto& wep = world.GetComponent<cmpt::GrenadeLauncher>(id);
            wep.explosion_damage += 1;
        }
    }
    
    void DequipGrenadeLauncher(strg::Registry& world, const Entity id) {
        if (world.HasComponent<cmpt::GrenadeLauncher>(id)) {
           world.RemoveComponent<cmpt::GrenadeLauncher>(id);
        }
    }
}