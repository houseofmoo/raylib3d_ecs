#pragma once

#include "raylib.h"
#include "storage/registry.h"
#include "data/loot.h"

namespace spwn::loot {
    void Exp(Storage::Registry& world, const Vector3 position, int exp_amount);
    void Money(Storage::Registry& world, const Vector3 position, int exp_amount);
    void Powerup(Storage::Registry& world, const Vector3 position, data::loot::PowerupKind kind);
    void Weapon(Storage::Registry& world, const Vector3 position, data::loot::WeaponKind kind);
}