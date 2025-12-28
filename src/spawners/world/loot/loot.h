#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::loot {
    void Exp(strg::Registry& world, const Vector3 position, int exp_amount, const float collider_scaler);
    void Money(strg::Registry& world, const Vector3 position, int exp_amount, const float collider_scaler);
    void Powerup(strg::Registry& world, const Vector3 position, data::loot::PowerupKind kind, const float collider_scaler);
    void Weapon(strg::Registry& world, const Vector3 position, data::loot::WeaponKind kind, const float collider_scaler);
    void WeaponCrate(strg::Registry& world, const Vector3 position, const float collider_scaler);

}