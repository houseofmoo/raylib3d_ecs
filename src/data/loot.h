#pragma once

#include "raylib.h"

namespace data::loot {
    enum class LootKind {
        Exp,
        Damage,
        PelletCount,
        AttackSpeed,
        Penetration,
        Richochette,

        MoveSpeed,
        DashDistance,
        PickupRange,
        Health,
        MaxHp,
        HpRegen,
    };

    enum class WeaponKind {
        Pistol,
        Shotgun,
        Rifle,
        Sniper,
        SMG,
        LMG,
        RocketLauncher,
    };

    enum class LootRarity {
        Common,
        Uncommon,
        Rare,
        Epic,
        Legendary,
    };

    enum class LootSource {
        Enemy,
        Crate,
    };

    enum class LootEffectType {
        Additive,
        Multiplicative,
    };

    enum class LootEffectTarget {
        Player,
        Weapon,
    };
}