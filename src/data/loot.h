#pragma once

#include <unordered_map>
#include "raylib.h"

namespace data::loot {
    enum class LootKind {
        Exp,        // all monsters drop this
        Money,      // some monsters drop this as consolation?
        Powerup,    // affect players modifiers
        Weapon,     // weapons that can be equiped
                    // if we get another of the same weapon have it upgrade some
                    // that for that weapon
        // Damage,
        // PelletCount,
        // AttackSpeed,
        // Penetration,
        // Richochette,

        // MoveSpeed,
        // DashDistance,
        // PickupRange,
        // Health,
        // MaxHp,
        // HpRegen,
    };

    // player global powerups -> affect player modifiers
    enum class PowerupKind {
        Damage,
        AttackSpeed,
        MoveSpeed,
        PickupRange,
        DashDistance,
        Health,
        MaxHp,
        Last
    };

    // allowed to carry 2 weapons + an equipment?
    enum class WeaponKind {
        Pistol,          // single pellet, starter wep expected to be not awesome
        Shotgun,         // spread pattern (3 pellets)
        Rifle,           // richochettes?
        Sniper,          // penetration (3 enemies)
        RailGun,         // high speed + penetration (5 enemies)
        SMG,             // high attack speed
        Grenade,         // arch + area damage
        RocketLauncher,  // slow moving + area damage
        Last
    };

    extern std::unordered_map<data::loot::PowerupKind, Color> g_powerup_colors;
    extern std::unordered_map<data::loot::WeaponKind, Color> g_weapon_colors;

    // enum class LootRarity {
    //     Common,
    //     Uncommon,
    //     Rare,
    //     Epic,
    //     Legendary,
    // };

    // enum class LootSource {
    //     Enemy,
    //     Crate,
    // };

    // enum class LootEffectType {
    //     Additive,
    //     Multiplicative,
    // };

    // enum class LootEffectTarget {
    //     Player,
    //     Weapon,
    // };
}