#include "systems/events/loot_pickedup_event_system.h"


#include "components/components.h"
#include "spawners/system/events/notification.h"
#include "spawners/equip/weapon/shotgun.h"
#include "spawners/equip/weapon/pistol.h"
#include "data/player/player.h"
#include "data/entity.h"
#include "utils/debug.h"

namespace sys::evt {
    void ApplyPowerup(Storage::Registry& world, data::loot::PowerupKind kind, Entity id) {
        switch (kind) {
            case data::loot::PowerupKind::Damage: {
                data::g_player.damage_multiplier += 0.1f;
                spwn::evt::Notification(world, "+DAMGE");
                break;
            }
            case data::loot::PowerupKind::AttackSpeed: {
                data::g_player.attack_speed_multiplier += 0.1f;
                spwn::evt::Notification(world, "+ATTACK SPEED");
                break;
            }
            case data::loot::PowerupKind::MoveSpeed: {
                data::g_player.move_speed_multiplier += 0.1f;
                spwn::evt::Notification(world, "+MOVE SPEED");
                break;
            }
            case data::loot::PowerupKind::PickupRange: {
                data::g_player.pickup_range_multiplier += 0.1f;
                spwn::evt::Notification(world, "+PICKUP RANGE");
                break;
            }
            case data::loot::PowerupKind::DashDistance: {
                data::g_player.dash_range_multiplier += 0.1f;
                spwn::evt::Notification(world, "+DASH RANGE");
                break;
            }
            case data::loot::PowerupKind::Health: {
                auto& hp = world.GetComponent<cmpt::Health>(data::g_player.id);
                if (hp.amount < hp.max) {
                    hp.amount += 10;
                    if (hp.amount > hp.max) hp.amount = hp.max;
                }
                spwn::evt::Notification(world, "+HP");
                break;
            }
            case data::loot::PowerupKind::MaxHp: {
                auto& hp = world.GetComponent<cmpt::Health>(data::g_player.id);
                hp.max += 10;
                hp.amount += 10;
                spwn::evt::Notification(world, "+MAX HP");
                break;
            }
            default: {
                PRINT("unknown powerup kind in ApplyPowerup(): ", (int)kind);
                break;
            }
        }
    }

    void ApplyWeapon(Storage::Registry& world, data::loot::WeaponKind kind, Entity id) {
        // TODO: if weapon exists, upgrade it
        // otherwise give baselist to player
        switch (kind) {
            case data::loot::WeaponKind::Pistol: {
                spwn::weapon::EquipPistol(world, id);
                spwn::evt::Notification(world, "+PISTOL");
                break;
            }
            case data::loot::WeaponKind::Shotgun: {
                spwn::weapon::EquipShotgun(world, id);
                spwn::evt::Notification(world, "+SHOTGUN");
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

    void HandleLootPickedupEvents(Storage::Registry& world) noexcept {
        PROFILE_SCOPE("HandleLootPickedupEvents()");
        for(auto entity : world.View<cmpt::LootEvent>()) {

            world.AddComponent<tag::Destroy>(entity);
            auto& evt = world.GetComponent<cmpt::LootEvent>(entity);
            switch (evt.kind) {
                case data::loot::LootKind::Exp: {
                    auto& exp = world.GetComponent<cmpt::ExpLoot>(entity);
                    int levelup = data::g_player.AddExp(exp.amount);
                    spwn::evt::Notification(world, "+EXP");
                    if (levelup > 0) {
                        spwn::evt::Notification(world, "LVLUP " + std::to_string(levelup));
                    }
                    break;
                }
                case data::loot::LootKind::Money: {
                    auto& money = world.GetComponent<cmpt::MoneyLoot>(entity);
                    data::g_player.money += money.amount;
                    spwn::evt::Notification(world, "+" + std::to_string(money.amount) + " GOLD");
                    break;
                }

                case data::loot::LootKind::Powerup: {
                    auto& pukind = world.GetComponent<cmpt::PowerupLoot>(entity);
                    ApplyPowerup(world, pukind.kind, evt.id);
                    break;
                }

                case data::loot::LootKind::Weapon: {
                    auto& wepkind = world.GetComponent<cmpt::WeaponLoot>(entity);
                    ApplyWeapon(world, wepkind.kind, evt.id);
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