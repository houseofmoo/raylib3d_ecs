#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "utils/debug.h"
#include "data/player/player.h"

namespace sys::evt {
    inline void ApplyPowerup(data::loot::PowerupKind kind) {
        switch (kind) {
            case data::loot::PowerupKind::Damage: {
                PRINT("+Damage powerup");
                data::player::g_player.damage_multiplier += 0.1f;
                break;
            }
            case data::loot::PowerupKind::AttackSpeed: {
                data::player::g_player.attack_speed_multiplier += 0.1f;
                PRINT("+AttackSpeed powerup");
                break;
            }
            case data::loot::PowerupKind::MoveSpeed: {
                data::player::g_player.move_speed_multiplier += 0.1f;
                PRINT("+MoveSpeed powerup");
                break;
            }
            case data::loot::PowerupKind::PickupRange: {
                data::player::g_player.pickup_range_multiplier += 0.1f;
                PRINT("+PickupRange powerup");
                break;
            }
            case data::loot::PowerupKind::DashDistance: {
                PRINT("+DashDistance powerup");
                break;
            }
            case data::loot::PowerupKind::Health: {
                PRINT("+Health powerup");
                break;
            }
            case data::loot::PowerupKind::MaxHp: {
                PRINT("+MaxHp powerup");
                break;
            }
            case data::loot::PowerupKind::HpRegen: {
                PRINT("+HpRegen powerup");
                break;
            }
            default: {
                PRINT("unknown powerup kind in ApplyPowerup(): ", (int)kind);
                break;
            }
        }
    }

       inline void ApplyWeapon(data::loot::WeaponKind kind) {
        switch (kind) {
            case data::loot::WeaponKind::Pistol: {
                PRINT("+Pistol");
                break;
            }
            case data::loot::WeaponKind::Shotgun: {
                PRINT("+Shotgun");
                break;
            }
            case data::loot::WeaponKind::Rifle: {
                PRINT("+Rifle");
                break;
            }
            case data::loot::WeaponKind::Sniper: {
                PRINT("+Sniper");
                break;
            }
            case data::loot::WeaponKind::RailGun: {
                PRINT("+RailGun");
                break;
            }
            case data::loot::WeaponKind::SMG: {
                PRINT("+SMG");
                break;
            }
            case data::loot::WeaponKind::Grenade: {
                PRINT("+Grenade");
                break;
            }
            case data::loot::WeaponKind::RocketLauncher: {
                PRINT("+RocketLauncher");
                break;
            }
            default: {
                PRINT("unknown wep kind in ApplyWeapon(): ", (int)kind);
                break;
            }
        }
    }

    inline void HandleLootPickedupEvents(Storage::Registry& world) noexcept {
        PROFILE_SCOPE("HandleLootPickedupEvents()");
        for(auto entity : world.View<cmpt::LootEvent>()) {
            world.AddComponent<tag::Destroy>(entity);
            auto& evt = world.GetComponent<cmpt::LootEvent>(entity);
            switch (evt.kind) {
                case data::loot::LootKind::Powerup: {
                    auto& pukind = world.GetComponent<cmpt::PowerupLoot>(entity);
                    ApplyPowerup(pukind.kind);
                    break;
                }

                case data::loot::LootKind::Weapon: {
                    auto& wepkind = world.GetComponent<cmpt::WeaponLoot>(entity);
                    ApplyWeapon(wepkind.kind);
                    PRINT("got weapon pickup");
                    break;
                }

                default: {
                    PRINT("Unknow loot kind in HandleLootPickedupEvents()");
                    break;
                }
            }
        }
    }

    
}