#pragma once

#include "raylib.h"
#include "data/entity.h"
#include "storage/registry.h"
#include "components/components.h"
#include "spawners/equip/weapon/weapons.h"
#include "utils/debug.h"

namespace spwn::enemy {
    Entity Grunt(
        strg::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp
    );

    inline void GruntWithAPistol(
        strg::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp
    ) {
        Entity id = Grunt(world, position, move_mode, hp);
        spwn::weapon::EquipPistol(
            world, 
            id, 
            data::cnst::ENEMY_PROJECTILE_LAYER, 
            data::cnst::ENEMY_PROJECTILE_LAYER_MASK
        );
    }

    Entity Brute(
        strg::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp
    );
}