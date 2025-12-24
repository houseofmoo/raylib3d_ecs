#include "spawners/world/loot/loot.h"
#include "raymath.h"
#include "data/entity.h"
#include "data/player/player.h"
#include "resources/asset_loader.h"
#include "components/components.h"
#include "components/cmpt_helpers.h"
#include "utils/position.h"

namespace spwn::loot {
    void Exp(Storage::Registry& world, const Vector3 position, int exp_amount) {
        Entity exp = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            exp, 
            cmpt::Loot{ .kind = data::loot::LootKind::Exp }
        );

        world.AddComponent<cmpt::ExpLoot>(
            exp,
            cmpt::ExpLoot { exp_amount }
        );

        world.AddComponent<cmpt::Transform>(
            exp,
            cmpt::Transform{ 
                .position = utils::GetRandomValidPisitionNearTarget(
                    Vector3{ position.x, data::cnst::EXP_SIZE.y * 0.5f, position.z },
                    1
                ),
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            exp,
            cmpt::RotateInPlace{ .speed = data::cnst::LOOT_ROTATION_SPEED }
        );

        world.AddComponent<cmpt::Collider>(
            exp,
            cmpt::Collider{
                .layer = data::cnst::LOOT_LAYER,
                .mask = data::cnst::LOOT_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    cmpt::MinLootColliderSize(data::cnst::EXP_SIZE),
                    data::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            exp,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::LOOT_LIFETIME 
            }
        );

        world.AddComponent<cmpt::Draw>(
            exp,
            cmpt::Draw{
                .size = data::cnst::EXP_SIZE, 
                .color = data::cnst::EXP_COLOR, 
                .model = &rsrc::asset::exp_model,
            }
        );
    }

    void Money(Storage::Registry& world, const Vector3 position, int money_amount) { 
        Entity money = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            money, 
            cmpt::Loot{ .kind = data::loot::LootKind::Money }
        );

        world.AddComponent<cmpt::MoneyLoot>(
            money,
            cmpt::MoneyLoot { money_amount }
        );

        world.AddComponent<cmpt::Transform>(
            money,
            cmpt::Transform{ 
                .position = utils::GetRandomValidPisitionNearTarget(
                    Vector3{ position.x, data::cnst::MONEY_SIZE.y * 0.5f, position.z },
                    1
                ),
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            money,
            cmpt::RotateInPlace{ .speed = data::cnst::LOOT_ROTATION_SPEED }
        );

        world.AddComponent<cmpt::Collider>(
            money,
            cmpt::Collider{
                .layer = data::cnst::LOOT_LAYER,
                .mask = data::cnst::LOOT_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    cmpt::MinLootColliderSize(data::cnst::MONEY_SIZE),
                    data::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            money,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::LOOT_LIFETIME 
            }
        );

        world.AddComponent<cmpt::Draw>(
            money,
            cmpt::Draw{
                .size = data::cnst::MONEY_SIZE, 
                .color = GOLD, 
                .model = &rsrc::asset::money_model,
            }
        );
    }

    void Powerup(Storage::Registry& world, const Vector3 position, data::loot::PowerupKind kind) {
        Entity powerup = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            powerup, 
            cmpt::Loot{ .kind = data::loot::LootKind::Powerup }
        );

        world.AddComponent<cmpt::PowerupLoot>(
            powerup,
            cmpt::PowerupLoot { .kind = kind }
        );

        world.AddComponent<cmpt::Transform>(
            powerup,
            cmpt::Transform{ 
                .position = utils::GetRandomValidPisitionNearTarget(
                    Vector3{ position.x, data::cnst::POWERUP_SIZE.y * 0.5f, position.z },
                    1
                ),
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            powerup,
            cmpt::RotateInPlace{ .speed = data::cnst::LOOT_ROTATION_SPEED }
        );

        world.AddComponent<cmpt::Collider>(
            powerup,
            cmpt::Collider{
                .layer = data::cnst::LOOT_LAYER,
                .mask = data::cnst::LOOT_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    cmpt::MinLootColliderSize(data::cnst::POWERUP_SIZE),
                    data::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            powerup,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::LOOT_LIFETIME 
            }
        );

        world.AddComponent<cmpt::Draw>(
            powerup,
            cmpt::Draw{
                .size = data::cnst::POWERUP_SIZE, 
                .color = data::cnst::POWERUP_COLORS[(int)kind],
                .model = &rsrc::asset::powerup_model,
            }
        );
    }

    void Weapon(Storage::Registry& world, const Vector3 position, data::loot::WeaponKind kind) {
        Entity weapon = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            weapon, 
            cmpt::Loot{ .kind = data::loot::LootKind::Weapon }
        );

        world.AddComponent<cmpt::WeaponLoot>(
            weapon,
            cmpt::WeaponLoot { .kind = kind }
        );

        world.AddComponent<cmpt::Transform>(
            weapon,
            cmpt::Transform{
                .position = utils::GetRandomValidPisitionNearTarget(
                    Vector3{ position.x, data::cnst::WEAPON_SIZE.y * 0.5f, position.z },
                    1
                ),
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            weapon,
            cmpt::RotateInPlace{ .speed = data::cnst::LOOT_ROTATION_SPEED }
        );

        world.AddComponent<cmpt::Collider>(
            weapon,
            cmpt::Collider{
                .layer = data::cnst::LOOT_LAYER,
                .mask = data::cnst::LOOT_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    cmpt::MinLootColliderSize(data::cnst::WEAPON_SIZE),
                    data::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            weapon,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::LOOT_LIFETIME
            }
        );

        world.AddComponent<cmpt::Draw>(
            weapon,
            cmpt::Draw{
                .size = data::cnst::WEAPON_SIZE, 
                .color = data::cnst::WEAPON_COLORS[(int)kind],
                .model = &rsrc::asset::weapon_model,
            }
        );
    }

    void WeaponCrate(Storage::Registry& world, const Vector3 position) {
        Entity entity = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            entity, 
            cmpt::Loot{ .kind = data::loot::LootKind::WeaponCrate }
        );

        world.AddComponent<cmpt::Transform>(
            entity,
            cmpt::Transform{
                .position = utils::GetRandomValidPisitionNearTarget(
                    Vector3{ position.x, data::cnst::WEAPON_CRATE_SIZE.y * 0.5f, position.z },
                    1
                ),
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            entity,
            cmpt::RotateInPlace{ .speed = data::cnst::LOOT_ROTATION_SPEED }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::LOOT_LAYER,
                .mask = data::cnst::LOOT_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    cmpt::MinLootColliderSize(data::cnst::WEAPON_CRATE_SIZE),
                    data::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            entity,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::LOOT_LIFETIME
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{
                .size = data::cnst::WEAPON_CRATE_SIZE, 
                .color = data::cnst::WEAPON_CRATE_COLOR,
                .model = &rsrc::asset::weapon_crate_model,
            }
        );

    }
}