#pragma once

#include "data/entity.h"
#include "storage/registry.h"

namespace spwn::weapon {
    //void Shotgun(Storage::Registry& world, const Entity parent);
    void EquipShotgun(Storage::Registry& world, const Entity id);
    void DequipShotgun(Storage::Registry& world, const Entity id);
}