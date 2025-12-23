#include "spawners/world/projectile/bullet.h"
#include "raymath.h"
#include "data/entity.h"
#include "resources/asset_loader.h"
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
                .layer = data::cnst::PROJECTILE_LAYER,
                .mask = data::cnst::PROJECTILE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::PROJECTILE_SIZE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            bullet,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::PROJECTILE_LIFETIME
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
                .size = data::cnst::PROJECTILE_SIZE, 
                .color = data::cnst::PROJECTILE_COLOR,
                .model = &rsrc::asset::bullet_model,
            }
        );
    }
}