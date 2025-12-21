#pragma once

#include "data/entity.h"
#include "storage/registry.h"

namespace spwn::weapon {
    void Pistol(Storage::Registry& world, Entity parent);
    void EquipPistol(Storage::Registry& world, Entity id);
}