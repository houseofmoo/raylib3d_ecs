#pragma once

#include "data/entity.h"
#include "storage/registry.h"

namespace spwn::weapon {
    void Shotgun(Storage::Registry& world, Entity parent);
    void EquipShotgun(Storage::Registry& world, Entity id);
}