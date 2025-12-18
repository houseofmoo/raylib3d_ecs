#include "spawners/loot/loot.h"
#include "raymath.h"
#include "data/entity.h"
#include "resources/assets.h"
#include "components/components.h"
#include "components/tags.h"

namespace spwn::loot {
    void Loot(Storage::Registry& world, const Vector3 position, data::loot::LootKind kind, Color color) {
        auto powerup = world.CreateEntity();

        world.AddComponent<cmpt::Loot>(
            powerup,
            cmpt::Loot{ .kind = kind }
        );

        world.AddComponent<cmpt::Transform>(
            powerup,
            cmpt::Transform{ 
                Vector3{ position.x, data::size::LOOT.y * 0.5f, position.z },
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::RotateInPlace>(
            powerup,
            cmpt::RotateInPlace{ 2.0f }
        );

        world.AddComponent<cmpt::Collider>(
            powerup,
            cmpt::Collider{
                .layer = data::layer::LOOT,
                .mask = data::layer::PLAYER,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::size::LOOT
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
                .size = data::size::LOOT, 
                .color = color, 
                .model = &rsrc::asset::loot_model,
            }
        );
    }
}