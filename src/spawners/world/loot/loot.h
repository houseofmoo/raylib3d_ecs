#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::loot {
    void Exp(strg::Registry& world, const Vector3 position, int exp_amount);
    void Money(strg::Registry& world, const Vector3 position, int exp_amount);
    void Powerup(strg::Registry& world, const Vector3 position, data::loot::PowerupKind kind);
    void Weapon(strg::Registry& world, const Vector3 position, data::loot::WeaponKind kind);
    void WeaponCrate(strg::Registry& world, const Vector3 position);

}