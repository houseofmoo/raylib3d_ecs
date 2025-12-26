#pragma once

#include "storage/registry.h"
#include "data/entity.h"

namespace spwn::weapon {
    void EquipGrenade(strg::Registry& world, const Entity id);
    void DequipGrenade(strg::Registry& world, const Entity id);
}