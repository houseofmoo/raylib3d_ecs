#pragma once

#include "data/entity.h"
#include "storage/registry.h"

namespace spwn::weapon {
    struct BaseWepConfig {
        Entity id;
        Layer layer;
        Layer mask;
    };

    void EquipPistol(strg::Registry& world, const Entity id, Layer layer, Layer mask);
    void DequipPistol(strg::Registry& world, const Entity id);

    void EquipShotgun(strg::Registry& world, const Entity id, Layer layer, Layer mask);
    void DequipShotgun(strg::Registry& world, const Entity id);

    void EquipRifle(strg::Registry& world, const Entity id, Layer layer, Layer mask);
    void DequipRifle(strg::Registry& world, const Entity id);

    // void EquipSniper(strg::Registry& world, const Entity id);
    // void DequipSniper(strg::Registry& world, const Entity id);

    // void EquipRailgun(strg::Registry& world, const Entity id);
    // void DequipRailgun(strg::Registry& world, const Entity id);

    void EquipSMG(strg::Registry& world, const Entity id, Layer layer, Layer mask);
    void DequipSMG(strg::Registry& world, const Entity id);

    void EquipGrenadeLauncher(strg::Registry& world, const Entity id, Layer layer, Layer mask);
    void DequipGrenadeLauncher(strg::Registry& world, const Entity id);

    // void EquipRocketLauncher(strg::Registry& world, const Entity id);
    // void DequipRocketLauncher(strg::Registry& world, const Entity id);
}