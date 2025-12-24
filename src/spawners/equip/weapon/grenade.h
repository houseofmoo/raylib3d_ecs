#pragma once

#include "storage/registry.h"
#include "data/entity.h"

namespace spwn::weapon {
    void EquipGrenade(Storage::Registry& world, const Entity id);
    void DequipGrenade(Storage::Registry& world, const Entity id);
}