#include "spawners/world/projectile/grenade.h"

#include "raymath.h"
#include "data/entity.h"
#include "resources/asset_loader.h"
#include "components/components.h"

namespace spwn::proj {
    void Grenade(Storage::Registry& world, const Vector3 start_pos,
                Vector3 end_pos, const int damage) {

        auto entity = world.CreateEntity();
        
        world.AddComponent<tag::Projectile>(
            entity,
            tag::Projectile{}
        );

        world.AddComponent<tag::DestroyOnTerrainCollision>(
            entity,
            tag::DestroyOnTerrainCollision{}
        );

        world.AddComponent<cmpt::Transform>(
            entity,
            cmpt::Transform{ 
                start_pos,
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::ArchMove>(
            entity,
            cmpt::ArchMove{
                .start = start_pos,
                .end = end_pos,
                .duration = data::cnst::ARCH_DURATION,
                .elapsed = 0.0f,
                .height = data::cnst::ARCH_MAX_HEIGHT
            }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::PROJECTILE_LAYER,
                .mask = data::cnst::PROJECTILE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::GRENADE_SIZE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            entity,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::ARCH_DURATION 
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ 
                .amount = damage,
                .penetration = 0
            }
        );

        world.AddComponent<cmpt::ExplodeOnDestroy>(
            entity,
            cmpt::ExplodeOnDestroy{
                .radius = 1.0f,
                .duration = 1.0f
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = data::cnst::GRENADE_SIZE, 
                .scale = data::cnst::BASE_SCALE,
                .color = data::cnst::PROJECTILE_COLOR,
                .model = &rsrc::asset::grenade_model,
            }
        );
    }
}