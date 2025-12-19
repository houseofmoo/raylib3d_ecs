#include "spawners/enemies/brute.h"

#include "raymath.h"
#include "data/entity.h"
#include "resources/assets.h"
#include "components/components.h"

namespace spwn::enemy {
    void Brute(Storage::Registry& world, const Vector3 position, const int hp) {

        auto brute = world.CreateEntity();

        // for drop in animation
        Vector3 start_position = Vector3{ position.x, 50.0f, position.z };
        Vector3 end_position = Vector3{ position.x, data::size::BRUTE.y * 0.5f, position.z };
        
        world.AddComponent<tag::Enemy>(
            brute,
            tag::Enemy{}
        );

        world.AddComponent<tag::DropsLoot>(
            brute,
            tag::DropsLoot{}
        );
        
        world.AddComponent<cmpt::Transform>(
            brute,
            cmpt::Transform{ 
                .position = start_position,
                .rotation = QuaternionIdentity()
            }
        );
        
        world.AddComponent<cmpt::Velocity>(
            brute,
            cmpt::Velocity{ 0.0f, 0.0f, 0.0f }
        );

        world.AddComponent<cmpt::MoveIntent>(
            brute,
            cmpt::MoveIntent{
                .type = cmpt::MoveIntentType::Melee,
                .direction = Vector3Zero()
            }
        );

        world.AddComponent<cmpt::Collider>(
            brute,
            cmpt::Collider{
                .layer = data::layer::ENEMY,
                .mask = data::layer::PLAYER | data::layer::PROJECTILE | data::layer::TERRAIN,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::size::MinColldierSize(data::size::BRUTE)
            }
        );

        world.AddComponent<cmpt::Health>(
            brute,
            cmpt::Health{ hp * 2 }
        );

        world.AddComponent<cmpt::DamageReceiver>(
            brute,
            cmpt::DamageReceiver{ 0 }
        );

        world.AddComponent<cmpt::Speed>(
            brute,
            cmpt::Speed{ 
                .speed = 3.5f, 
                .speed_multiplier = 1.0f, 
                .dash_multiplier = 1.0f,
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            brute,
            cmpt::DamageDealer{10}
        );

        world.AddComponent<cmpt::SpawnAnimation>(
            brute,
            cmpt::SpawnAnimation{
                .start_position = start_position,
                .end_position = end_position
            }
        );
        
        world.AddComponent<cmpt::Draw>(
            brute,
            cmpt::Draw{ 
                .size = data::size::BRUTE, 
                .color = Color{ 255, 100, 255, 255 }, 
                .model = &rsrc::asset::brute_model,
            }
        );
    }
}