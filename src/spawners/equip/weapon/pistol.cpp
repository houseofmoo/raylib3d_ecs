#include "spawners/equip/weapon/pistol.h"
#include "components/components.h"

namespace spwn::weapon {
    // void Pistol(Storage::Registry& world, const Entity parent) {
    //     auto weapon = world.CreateEntity();
        
    //     world.AddComponent<cmpt::Weapon>(
    //         weapon,
    //         cmpt::Weapon{ 
    //             .parent = parent,
    //             .kind = data::loot::WeaponKind::Pistol 
    //         }
    //     );

    //     world.AddComponent<cmpt::Pistol>(
    //         weapon,
    //         cmpt::Pistol{
    //             .base_stats = cmpt::WeaponBaseStats {
    //                 .parent = parent,
    //                 .kind = data::loot::WeaponKind::Pistol,
    //                 .cooldown = 1.0f, 
    //                 .countdown = 1.0f,
    //                 .projectile_speed = 25.0f,
    //                 .damage = 10,
    //             }
    //         }
    //     );
    // }

    void EquipPistol(Storage::Registry& world, const Entity id) {
        if (!world.HasComponent<cmpt::Pistol>(id)) {
            world.AddComponent<cmpt::Pistol>(
                id,
                cmpt::Pistol{
                    .base_stats = cmpt::WeaponBaseStats {
                        .parent = id,
                        .kind = data::loot::WeaponKind::Pistol,
                        .cooldown = data::cnst::PISTOL_COOLDOWN, 
                        .countdown = data::cnst::PISTOL_COOLDOWN,
                        .projectile_speed = data::cnst::PISTOL_PROJECTILE_SPEED,
                        .damage = data::cnst::PISTOL_DAMAGE,
                    },
                }
            );
        } else {
            // TODO: upgrade random stat
            auto& wep = world.GetComponent<cmpt::Pistol>(id);
            wep.base_stats.damage += 1;
        }
    }

    void DequipPistol(Storage::Registry& world, const Entity id) {
        if (world.HasComponent<cmpt::Pistol>(id)) {
            world.RemoveComponent<cmpt::Pistol>(id);
        }
    }
}