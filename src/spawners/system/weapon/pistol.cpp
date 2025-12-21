#include "spawners/system/weapon/pistol.h"
#include "components/components.h"
#include "data/loot.h"

namespace spwn::weapon {
    void Pistol(Storage::Registry& world, Entity parent) {
        auto weapon = world.CreateEntity();
        
        world.AddComponent<tag::Weapon>(
            weapon,
            tag::Weapon{}
        );

        world.AddComponent<cmpt::WeaponStats>(
            weapon,
            cmpt::WeaponStats{
                .parent = parent,
                .kind = data::loot::WeaponKind::Pistol,
                .cooldown = 1.0f, 
                .countdown = 1.0f,
                .projectile_speed = 25.0f,
                .damage = 10,
            }
        );
    }
}