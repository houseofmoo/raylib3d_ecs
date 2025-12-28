#include "systems/events/loot_pickedup_event_system.h"


#include "components/components.h"
#include "spawners/system/events/notification.h"
#include "spawners/equip/weapon/weapons.h"
#include "data/player/player.h"
#include "data/game/game.h"
#include "data/entity.h"
#include "utils/debug.h"

namespace sys::evt {
    void ApplyPowerup(strg::Registry& world, data::loot::PowerupKind kind) {
        switch (kind) {
            case data::loot::PowerupKind::Damage: {
                data::g_player.damage_multiplier += data::cnst::DAMAGE_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_DAMAGE);
                break;
            }
            case data::loot::PowerupKind::AttackSpeed: {
                data::g_player.attack_speed_multiplier += data::cnst::ATTACK_SPEED_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_ATTACK_SPEED);
                break;
            }
            case data::loot::PowerupKind::MoveSpeed: {
                data::g_player.move_speed_multiplier += data::cnst::MOVE_SPEED_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_MOVE_SPEED);
                break;
            }
            case data::loot::PowerupKind::PickupRange: {
                data::g_player.pickup_range_multiplier += data::cnst::PICKUP_RANGE_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_PICKUP_RANGE);
                break;
            }
            case data::loot::PowerupKind::DashDistance: {
                data::g_player.dash_range_multiplier += data::cnst::DASH_DISTANCE_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_DASH_RANGE);
                break;
            }
            case data::loot::PowerupKind::Health: {
                auto& hp = world.GetComponent<cmpt::Health>(data::g_player.id);
                hp.amount += data::cnst::HP_POWERUP_VALUE;
                if (hp.amount > hp.max) hp.amount = hp.max;
                spwn::evt::Notification(world, data::notif::GAIN_HEALTH);
                break;
            }
            case data::loot::PowerupKind::MaxHp: {
                auto& hp = world.GetComponent<cmpt::Health>(data::g_player.id);
                hp.max += data::cnst::HPMAX_POWERUP_VALUE;
                hp.amount += data::cnst::HPMAX_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_MAX_HEALTH);
                break;
            }
            default: {
                PRINT("unknown powerup kind in ApplyPowerup(): ", (int)kind);
                break;
            }
        }
    }

    void ApplyWeapon(strg::Registry& world, data::loot::WeaponKind kind, Entity id) {
        // TODO: if weapon exists, upgrade it
        // otherwise give baselist to player
        switch (kind) {
            case data::loot::WeaponKind::Pistol: {
                spwn::weapon::EquipPistol(
                    world, 
                    id, 
                    data::cnst::PLAYER_PROJECTILE_LAYER, 
                    data::cnst::PLAYER_PROJECTILE_LAYER_MASK
                );
                spwn::evt::Notification(world, data::notif::GAIN_PISTOL);
                break;
            }
            case data::loot::WeaponKind::Shotgun: {
                spwn::weapon::EquipShotgun(
                    world, 
                    id, 
                    data::cnst::PLAYER_PROJECTILE_LAYER, 
                    data::cnst::PLAYER_PROJECTILE_LAYER_MASK
                );
                spwn::evt::Notification(world, data::notif::GAIN_SHOTGUN);
                break;
            }
            case data::loot::WeaponKind::Rifle: {
                spwn::weapon::EquipRifle(
                    world, 
                    id, 
                    data::cnst::PLAYER_PROJECTILE_LAYER, 
                    data::cnst::PLAYER_PROJECTILE_LAYER_MASK
                );
                spwn::evt::Notification(world, data::notif::GAIN_RIFLE);
                break;
            }
            case data::loot::WeaponKind::Sniper: {
                spwn::evt::Notification(world, data::notif::GAIN_SNIPER);
                break;
            }
            case data::loot::WeaponKind::RailGun: {
                spwn::evt::Notification(world, data::notif::GAIN_RAILGUN);
                break;
            }
            case data::loot::WeaponKind::SMG: {
                spwn::weapon::EquipSMG(
                    world, 
                    id, 
                    data::cnst::PLAYER_PROJECTILE_LAYER, 
                    data::cnst::PLAYER_PROJECTILE_LAYER_MASK
                );
                spwn::evt::Notification(world, data::notif::GAIN_SMG);
                break;
            }
            case data::loot::WeaponKind::GrenadeLauncher: {
                spwn::weapon::EquipGrenadeLauncher(
                    world, 
                    id, 
                    data::cnst::PLAYER_PROJECTILE_LAYER, 
                    data::cnst::PLAYER_PROJECTILE_LAYER_MASK
                );
                spwn::evt::Notification(world, data::notif::GAIN_GRENADE_LAUNCHER);
                break;
            }
            case data::loot::WeaponKind::RocketLauncher: {
                spwn::evt::Notification(world, data::notif::GAIN_ROCKET_LAUNCHER);
                break;
            }
            default: {
                PRINT("unknown wep kind in ApplyWeapon(): ", (int)kind);
                break;
            }
        }
    }

    void HandleLootPickedupEvents(strg::Registry& world) noexcept {
        PROFILE_SCOPE("HandleLootPickedupEvents()");
        for(auto entity : world.View<cmpt::LootEvent>()) {

            world.AddComponent<tag::Destroy>(entity);
            auto& evt = world.GetComponent<cmpt::LootEvent>(entity);
            switch (evt.kind) {
                case data::loot::LootKind::Exp: {
                    auto& exp = world.GetComponent<cmpt::ExpLoot>(entity);
                    int levelup = data::g_player.AddExp(exp.amount);
                    spwn::evt::Notification(world, data::notif::GAIN_EXP);
                    if (levelup > 0) {
                        spwn::evt::Notification(world, data::notif::GAIN_LEVELUP);
                    }
                    break;
                }
                case data::loot::LootKind::Money: {
                    auto& money = world.GetComponent<cmpt::MoneyLoot>(entity);
                    data::g_player.money += money.amount;
                    spwn::evt::Notification(world, data::notif::GAIN_MONEY);
                    break;
                }

                case data::loot::LootKind::Powerup: {
                    auto& pukind = world.GetComponent<cmpt::PowerupLoot>(entity);
                    ApplyPowerup(world, pukind.kind);
                    break;
                }

                case data::loot::LootKind::Weapon: {
                    PRINT("old, delete me - found weapon drop")
                    auto& wepkind = world.GetComponent<cmpt::WeaponLoot>(entity);
                    ApplyWeapon(world, wepkind.kind, evt.id);
                    break;
                }

                case data::loot::LootKind::WeaponCrate: {
                    data::g_weapon_select_menu.show = true;
                    PRINT("+wep crate");
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