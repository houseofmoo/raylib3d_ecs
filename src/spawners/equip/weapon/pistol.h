#pragma once

#include "data/entity.h"
#include "storage/registry.h"

namespace spwn::weapon {
    // void Pistol(strg::Registry& world, const Entity parent);
    void EquipPistol(strg::Registry& world, const Entity id);
    void DequipPistol(strg::Registry& world, const Entity id);
}