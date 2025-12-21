#include "spawners/equip/weapon/shotgun.h"
#include "components/components.h"
#include "data/loot.h"
#include "utils/debug.h"

namespace spwn::weapon {
    void Shotgun(Storage::Registry& world, Entity parent) {
        auto weapon = world.CreateEntity();
        world.AddComponent<cmpt::Weapon>(
            weapon,
            cmpt::Weapon{ 
                .parent = parent,
                .kind = data::loot::WeaponKind::Shotgun 
            }
        );

        world.AddComponent<cmpt::Shotgun>(
            weapon,
            cmpt::Shotgun{
                .base_stats = cmpt::WeaponBaseStats {
                    .parent = parent,
                    .kind = data::loot::WeaponKind::Shotgun,
                    .cooldown = 1.0f, 
                    .countdown = 1.0f,
                    .projectile_speed = 25.0f,
                    .damage = 10,
                },
                .spread_deg = 30,
                .pellet_count = 3
            }
        );
    }

    void EquipShotgun(Storage::Registry& world, Entity id) {
        // if entity does not have a shotgun, give them one
        if (!world.HasComponent<cmpt::Shotgun>(id)) {
            world.AddComponent<cmpt::Shotgun>(
                id,
                cmpt::Shotgun{
                    .base_stats = cmpt::WeaponBaseStats {
                        .parent = id,
                        .kind = data::loot::WeaponKind::Shotgun,
                        .cooldown = 2.5f, 
                        .countdown = 1.0f,
                        .projectile_speed = 20.0f,
                        .damage = 9,
                    },
                    .spread_deg = 30,
                    .pellet_count = 3
                }
            );
            return;
        }

        // upgrade random stat on the shotgun
        auto& wep = world.GetComponent<cmpt::Shotgun>(id);
        wep.base_stats.damage += 1;
    }
}