#include "spawners/projectile/grenade.h"

#include "raymath.h"
#include "data/entity.h"
#include "resources/assets.h"
#include "components/components.h"
#include "components/tags.h"

namespace spwn::proj {
    void Grenade(Storage::Registry& world, const Vector3 start_pos,
                Vector3 end_pos, const int damage) {

        auto grenade = world.CreateEntity();
        
        world.AddComponent<tag::Projectile>(
            grenade,
            tag::Projectile{}
        );

        world.AddComponent<cmpt::Transform>(
            grenade,
            cmpt::Transform{ 
                start_pos,
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::ArchMove>(
            grenade,
            cmpt::ArchMove{
                .start = start_pos,
                .end = end_pos,
                .duration = 1.0f,
                .elapsed = 0.0f,
                .height = 5.0f
            }
        );

        world.AddComponent<cmpt::Collider>(
            grenade,
            cmpt::Collider{
                .layer = data::layer::PROJECTILE,
                .mask = data::layer::ENEMY | data::layer::TERRAIN,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::size::PROJECTILE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            grenade,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = 1.0f 
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            grenade,
            cmpt::DamageDealer{ 
                .amount = damage,
                .penetration = 0
            }
        );

        world.AddComponent<cmpt::DestroyOnContact>(
            grenade,
            cmpt::DestroyOnContact { 1 }
        );

        world.AddComponent<cmpt::Draw>(
            grenade,
            cmpt::Draw{ 
                .size = data::size::PROJECTILE, 
                .color = ORANGE,
                .model = &rsrc::asset::bullet_model,
            }
        );
    }
}