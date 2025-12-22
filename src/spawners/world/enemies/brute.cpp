#include "spawners/world/enemies/brute.h"

#include "raymath.h"
#include "data/entity.h"
#include "resources/asset_loader.h"
#include "components/attach.h"

namespace spwn::enemy {
    void Brute(
            Storage::Registry& world, 
            const Vector3 position, 
            const cmpt::MoveIntentType move_type, 
            const int hp) {

        auto brute = world.CreateEntity();

        // for drop in animation
        Vector3 start_position = Vector3{ position.x, 50.0f, position.z };
        Vector3 end_position = Vector3{ position.x, data::size::BRUTE.y * 0.5f, position.z };
        
        world.AddComponent<tag::Enemy>(
            brute,
            tag::Enemy{}
        );

        world.AddComponent<cmpt::DropsLoot>(
            brute,
            cmpt::DropsLoot{ 1.5f }
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

        cmpt::AttachMovementComponent(world, brute, move_type);

        world.AddComponent<cmpt::MoveIntent>(
            brute,
            cmpt::MoveIntent{
                .type = move_type,
                .direction = Vector3Zero(),
                .start_rotation = QuaternionIdentity(),
                .rotation_complete = true,
                .rotation_duration = 0.2f,
                .rotation_elapsed = 0.0f
            }
        );

        cmpt::AttachColliderComponent(
            world, 
            brute, 
            cmpt::Collider{
                .layer = data::layer::ENEMY,
                .mask = data::layer::PLAYER | data::layer::ENEMY | data::layer::PROJECTILE,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::size::MinColldierSize(data::size::BRUTE)
            }
        );

        int max_hp = (int)(hp * 1.5);
        world.AddComponent<cmpt::Health>(
            brute,
            cmpt::Health{ max_hp,  max_hp }
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