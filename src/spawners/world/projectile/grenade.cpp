#include "spawners/world/projectile/grenade.h"

#include "raymath.h"
#include "data/entity.h"
#include "assets/assets.h"
#include "components/components.h"

namespace spwn::proj {
    void Grenade(
        strg::Registry& world, 
        const Vector3 start_pos,
        const Vector3 end_pos, 
        const GrenadeConfig& config) {

        auto entity = world.CreateEntity();
        
        world.AddComponent<cmpt::Projectile>(
            entity,
            cmpt::Projectile{
                .penetration = config.penetration
            }
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
                .duration = config.arch_duration,
                .elapsed = 0.0f,
                .height = config.arch_max_height
            }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = config.layer,
                .mask = config.mask,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::GRENADE_SIZE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            entity,
            cmpt::Lifetime{ 
                .countdown = config.arch_duration
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ 
                .amount = config.damage,
            }
        );

        world.AddComponent<cmpt::ExplodeOnDestroy>(
            entity,
            cmpt::ExplodeOnDestroy{
                .start_size = config.explosion_start_size,
                .end_size = config.explosion_end_size,
                .damage = config.explosion_damage,
                .duration = config.explosion_duration,
                .knockback_scale = config.explosion_knockback_scale,
                .knockback_duration = config.explosion_duration,
                .layer = config.layer,
                .mask = config.mask,
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = data::cnst::GRENADE_SIZE, 
                .scale = data::cnst::BASE_SCALE,
                .color = data::cnst::GRENADE_COLOR,
                .model = &asset::models[asset::ModelType::Grenade],
            }
        );
    }
}