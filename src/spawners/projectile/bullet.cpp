#include "spawners/projectile/bullet.h"
#include "raymath.h"
#include "data/entity.h"
#include "resources/assets.h"
#include "components/components.h"

namespace spwn::proj {
    void Bullet(Storage::Registry& world, const Vector3 position, const Vector3 direction,
                const int damage, const int penetration) {
        
        auto bullet = world.CreateEntity();
        
        world.AddComponent<tag::Projectile>(
            bullet,
            tag::Projectile{}
        );

        world.AddComponent<tag::DestroyOnTerrainCollision>(
            bullet,
            tag::DestroyOnTerrainCollision{}
        );

        world.AddComponent<cmpt::Transform>(
            bullet,
            cmpt::Transform{ 
                position,
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::Velocity>(
            bullet,
            cmpt::Velocity{ direction.x, 0.0f, direction.z }
        );

        world.AddComponent<cmpt::Collider>(
            bullet,
            cmpt::Collider{
                .layer = data::layer::PROJECTILE,
                .mask = data::layer::ENEMY | data::layer::TERRAIN,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::size::PROJECTILE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            bullet,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = 10.0f 
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            bullet,
            cmpt::DamageDealer{ damage }
        );

        world.AddComponent<cmpt::Penetration>(
            bullet,
            cmpt::Penetration { penetration }
        );

        world.AddComponent<cmpt::Draw>(
            bullet,
            cmpt::Draw{ 
                .size = data::size::PROJECTILE, 
                .color = RED,
                .model = &rsrc::asset::bullet_model,
            }
        );
    }
}