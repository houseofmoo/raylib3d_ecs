#include "spawners/loot/loot.h"
#include "raymath.h"
#include "data/entity.h"
#include "data/player/player.h"
#include "resources/assets.h"
#include "components/components.h"

namespace spwn::loot {
    Vector3 FuzzPosition(Vector3 position) {
        // fuzz position x/z by -1,1
        int x = GetRandomValue(-10, 10);
        int z = GetRandomValue(-10, 10);
        return Vector3 { position.x + (x*0.1f), position.y, position.z + (z*0.1f)};
    }

    void Exp(Storage::Registry& world, const Vector3 position) {
        Entity exp = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            exp, 
            cmpt::Loot{ .kind = data::loot::LootKind::Exp }
        );

        world.AddComponent<cmpt::Transform>(
            exp,
            cmpt::Transform{ 
                .position = FuzzPosition(Vector3{ position.x, data::size::EXP.y * 0.5f, position.z }),
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            exp,
            cmpt::RotateInPlace{ .speed = 2.5f }
        );

        world.AddComponent<cmpt::Collider>(
            exp,
            cmpt::Collider{
                .layer = data::layer::LOOT,
                .mask = data::layer::PLAYER,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    data::size::MinColldierSize(data::size::EXP),
                    data::player::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            exp,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = 15.0f 
            }
        );

        world.AddComponent<cmpt::Draw>(
            exp,
            cmpt::Draw{
                .size = data::size::EXP, 
                .color = SKYBLUE, 
                .model = &rsrc::asset::exp_model,
            }
        );
    }

    void Money(Storage::Registry& world, const Vector3 position) { 
        Entity money = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            money, 
            cmpt::Loot{ .kind = data::loot::LootKind::Money }
        );

        world.AddComponent<cmpt::Transform>(
            money,
            cmpt::Transform{ 
                .position = FuzzPosition(Vector3{ position.x, data::size::MONEY.y * 0.5f, position.z }),
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            money,
            cmpt::RotateInPlace{ .speed = 2.5f }
        );

        world.AddComponent<cmpt::Collider>(
            money,
            cmpt::Collider{
                .layer = data::layer::LOOT,
                .mask = data::layer::PLAYER,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    data::size::MinColldierSize(data::size::MONEY),
                    data::player::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            money,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = 15.0f 
            }
        );

        world.AddComponent<cmpt::Draw>(
            money,
            cmpt::Draw{
                .size = data::size::MONEY, 
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

        // world.AddComponent<cmpt::Powerup>(
        //     powerup,
        //     cmpt::Powerup { .kind = kind }
        // );

        world.AddComponent<cmpt::Transform>(
            powerup,
            cmpt::Transform{ 
                .position = Vector3{ position.x, data::size::POWERUP.y * 0.5f, position.z },
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            powerup,
            cmpt::RotateInPlace{ .speed = 2.5f }
        );

        world.AddComponent<cmpt::Collider>(
            powerup,
            cmpt::Collider{
                .layer = data::layer::LOOT,
                .mask = data::layer::PLAYER,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    data::size::MinColldierSize(data::size::POWERUP),
                    data::player::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            powerup,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = 15.0f 
            }
        );

        world.AddComponent<cmpt::Draw>(
            powerup,
            cmpt::Draw{
                .size = data::size::POWERUP, 
                .color = data::loot::g_powerup_colors[kind],
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

        // world.AddComponent<cmpt::Weapon>(
        //     powerup,
        //     cmpt::Weapon { .kind = kind }
        // );

        world.AddComponent<cmpt::Transform>(
            weapon,
            cmpt::Transform{ 
                .position = Vector3{ position.x, data::size::WEAPON.y * 0.5f, position.z },
                .rotation = QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            weapon,
            cmpt::RotateInPlace{ .speed = 2.5f }
        );

        world.AddComponent<cmpt::Collider>(
            weapon,
            cmpt::Collider{
                .layer = data::layer::LOOT,
                .mask = data::layer::PLAYER,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = Vector3Scale(
                    data::size::MinColldierSize(data::size::WEAPON),
                    data::player::g_player.pickup_range_multiplier
                )
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            weapon,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = 15.0f 
            }
        );

        world.AddComponent<cmpt::Draw>(
            weapon,
            cmpt::Draw{
                .size = data::size::WEAPON, 
                .color = data::loot::g_weapon_colors[kind],
                .model = &rsrc::asset::weapon_model,
            }
        );
    }
}