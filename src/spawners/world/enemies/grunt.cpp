#include "spawners/world/enemies/grunt.h"

#include "raymath.h"
#include "data/entity.h"
#include "components/attach.h"
#include "resources/asset_loader.h"

namespace spwn::enemy {
    void Grunt(
            Storage::Registry& world, 
            const Vector3 position, 
            const cmpt::AIMoveMode move_mode, 
            const int hp) {

        auto enemy = world.CreateEntity();

        // for drop in animation
        Vector3 start_position = Vector3{ position.x, 50.0f, position.z };
        Vector3 end_position = Vector3{ position.x, data::size::GRUNT.y * 0.5f, position.z };
        
        world.AddComponent<tag::Enemy>(
            enemy,
            tag::Enemy{}
        );

        world.AddComponent<cmpt::DropsLoot>(
            enemy,
            cmpt::DropsLoot{ 1.0f }
        );
        
        world.AddComponent<cmpt::Transform>(
            enemy,
            cmpt::Transform{ 
                .position = start_position,
                .rotation = QuaternionIdentity()
            }
        );
        
        world.AddComponent<cmpt::Velocity>(
            enemy,
            cmpt::Velocity{ 0.0f, 0.0f, 0.0f }
        );

        cmpt::AttachMovementComponent(world, enemy, move_mode);
        world.AddComponent<cmpt::AIMoveIntent>(
            enemy,
            cmpt::AIMoveIntent{
                .mode = move_mode,
                .direction = Vector3Zero(),
                .start_rotation = QuaternionIdentity(),
                .rotation_complete = true,
                .rotation_duration = 0.2f,
                .rotation_elapsed = 0.0f,
                .stuck = false,
            }
        );

        cmpt::AttachColliderComponent(
            world,
            enemy,
            cmpt::Collider{
                .layer = data::layer::ENEMY,
                .mask = data::layer::PLAYER | data::layer::ENEMY | data::layer::PROJECTILE,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::size::MinColldierSize(data::size::GRUNT)
            }
        );

        world.AddComponent<cmpt::Health>(
            enemy,
            cmpt::Health{ hp, hp }
        );

        world.AddComponent<cmpt::DamageReceiver>(
            enemy,
            cmpt::DamageReceiver{ 0 }
        );

        world.AddComponent<cmpt::Speed>(
            enemy,
            cmpt::Speed{ 
                .speed = 5.0f, 
                .speed_multiplier = 1.0f, 
                .dash_multiplier = 1.0f,
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            enemy,
            cmpt::DamageDealer{5}
        );

        world.AddComponent<cmpt::SpawnAnimation>(
            enemy,
            cmpt::SpawnAnimation{
                .start_position = start_position,
                .end_position = end_position
            }
        );
        
        world.AddComponent<cmpt::Draw>(
            enemy,
            cmpt::Draw{ 
                .size = data::size::GRUNT, 
                .color = MAGENTA, 
                .model = &rsrc::asset::grunt_model,
            }
        );
    }
}