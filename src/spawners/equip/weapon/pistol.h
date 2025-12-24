#pragma once

#include "data/entity.h"
#include "storage/registry.h"

namespace spwn::weapon {
    // void Pistol(Storage::Registry& world, const Entity parent);
    void EquipPistol(Storage::Registry& world, const Entity id);
    void DequipPistol(Storage::Registry& world, const Entity id);
}