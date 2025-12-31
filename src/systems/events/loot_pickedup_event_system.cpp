#include "systems/events/loot_pickedup_event_system.h"


#include "components/components.h"
#include "spawners/system/events/notification.h"
#include "spawners/equip/weapon/weapons.h"
#include "assets/assets.h"
#include "sound/sound_player.h"
#include "data/game/game.h"
#include "data/entity.h"
#include "utils/debug.h"

namespace sys::evt {
    void UpgradeStats(strg::Registry& world, Entity id) {
        auto* stats = world.TryGetComponent<cmpt::Stats>(id);
        stats->damage_modifier += data::cnst::LEVEL_UP_DAMAGE_VALUE;
        stats->attack_speed_modifier += data::cnst::LEVEL_UP_ATTACK_SPEED_VALUE;
        stats->move_speed_modifier += data::cnst::LEVEL_UP_MOVE_SPEED_VALUE;
        stats->pickup_range_modifier += data::cnst::LEVEL_UP_PICKUP_RANGE_VALUE;
        stats->dash_speed_modifier += data::cnst::LEVEL_UP_DASH_DISTANCE_VALUE;
    }

    void ApplyExp(strg::Registry& world, Entity id, int exp_amount) {
        auto* player = world.TryGetComponent<cmpt::Player>(id);
        if (player == nullptr) return;

        player->exp += exp_amount;
        if (player->exp > player->exp_to_level) {
            while (player->exp >= player->exp_to_level) {
                player->exp  = player->exp - player->exp_to_level;
                player->level += 1;
                player->exp_to_level *= data::cnst::PLAYER_EXP_MODIFIER;
                spwn::evt::Notification(world, data::notif::GAIN_LEVELUP);
                UpgradeStats(world, id);
            }
        }
    }

    void ApplyPowerup(strg::Registry& world, data::loot::PowerupKind kind, Entity id) {
        auto* stats = world.TryGetComponent<cmpt::Stats>(id);
        if (stats == nullptr) return;

        switch (kind) {
            case data::loot::PowerupKind::Damage: {
                stats->damage_modifier += data::cnst::DAMAGE_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_DAMAGE);
                break;
            }
            case data::loot::PowerupKind::AttackSpeed: {
                stats->attack_speed_modifier += data::cnst::ATTACK_SPEED_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_ATTACK_SPEED);
                break;
            }
            case data::loot::PowerupKind::MoveSpeed: {
                stats->move_speed_modifier += data::cnst::MOVE_SPEED_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_MOVE_SPEED);
                break;
            }
            case data::loot::PowerupKind::PickupRange: {
                stats->pickup_range_modifier += data::cnst::PICKUP_RANGE_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_PICKUP_RANGE);
                break;
            }
            case data::loot::PowerupKind::DashDistance: {
                stats->dash_speed_modifier += data::cnst::DASH_DISTANCE_POWERUP_VALUE;
                spwn::evt::Notification(world, data::notif::GAIN_DASH_RANGE);
                break;
            }
            case data::loot::PowerupKind::Health: {
                stats->current_hp += data::cnst::HP_POWERUP_VALUE;
                if (stats->current_hp > stats->max_hp) stats->current_hp = stats->max_hp;
                spwn::evt::Notification(world, data::notif::GAIN_HEALTH);
                break;
            }
            case data::loot::PowerupKind::MaxHp: {
                stats->max_hp += data::cnst::HPMAX_POWERUP_VALUE;
                stats->current_hp += data::cnst::HPMAX_POWERUP_VALUE;
                if (stats->current_hp > stats->max_hp) stats->current_hp = stats->max_hp;
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
        // otherwise give baselist to entity
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
                    ApplyExp(world, evt.id, exp.amount);
                    spwn::evt::Notification(world, data::notif::GAIN_EXP);
                    snd::PlaySoundFxGlobal(asset::SoundFxType::PickupExp);
                    break;
                }
                case data::loot::LootKind::Money: {
                    auto& money = world.GetComponent<cmpt::MoneyLoot>(entity);
                    if (auto* player = world.TryGetComponent<cmpt::Player>(evt.id)) {
                        player->money += money.amount;
                        spwn::evt::Notification(world, data::notif::GAIN_MONEY);
                        snd::PlaySoundFxGlobal(asset::SoundFxType::PickupMoney);
                    }
                    break;
                }

                case data::loot::LootKind::Powerup: {
                    auto& pukind = world.GetComponent<cmpt::PowerupLoot>(entity);
                    ApplyPowerup(world, pukind.kind, evt.id);
                    snd::PlaySoundFxGlobal(asset::SoundFxType::PickupPowerup);
                    break;
                }

                case data::loot::LootKind::Weapon: {
                    PRINT("old, delete me - found weapon drop")
                    auto& wepkind = world.GetComponent<cmpt::WeaponLoot>(entity);
                    ApplyWeapon(world, wepkind.kind, evt.id);
                    snd::PlaySoundFxGlobal(asset::SoundFxType::PickupWeaponCrate);
                    break;
                }

                case data::loot::LootKind::WeaponCrate: {
                    data::g_weapon_select_menu.show = true;
                    snd::PlaySoundFxGlobal(asset::SoundFxType::PickupWeaponCrate);
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