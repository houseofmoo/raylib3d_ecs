#include "spawners/world/projectile/grenade.h"

#include "raymath.h"
#include "data/entity.h"
#include "resources/asset_loader.h"
#include "components/components.h"

namespace spwn::proj {
    void Grenade(Storage::Registry& world, const Vector3 start_pos,
                Vector3 end_pos, const int damage) {

        auto grenade = world.CreateEntity();
        
        world.AddComponent<tag::Projectile>(
            grenade,
            tag::Projectile{}
        );

        world.AddComponent<tag::DestroyOnTerrainCollision>(
            grenade,
            tag::DestroyOnTerrainCollision{}
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
                .duration = data::cnst::ARCH_DURATION,
                .elapsed = 0.0f,
                .height = data::cnst::ARCH_MAX_HEIGHT
            }
        );

        world.AddComponent<cmpt::Collider>(
            grenade,
            cmpt::Collider{
                .layer = data::cnst::PROJECTILE_LAYER,
                .mask = data::cnst::PROJECTILE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::PROJECTILE_SIZE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            grenade,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::ARCH_LIFETIME 
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            grenade,
            cmpt::DamageDealer{ 
                .amount = damage,
                .penetration = 0
            }
        );

        world.AddComponent<cmpt::Draw>(
            grenade,
            cmpt::Draw{ 
                .size = data::cnst::PROJECTILE_SIZE, 
                .color = data::cnst::PROJECTILE_COLOR,
                .model = &rsrc::asset::bullet_model,
            }
        );
    }
}