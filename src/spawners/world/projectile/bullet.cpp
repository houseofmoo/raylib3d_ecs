#include "spawners/world/projectile/bullet.h"
#include "raymath.h"
#include "data/entity.h"
#include "resources/asset_loader.h"
#include "components/components.h"

namespace spwn::proj {
    void Bullet(strg::Registry& world, const BulletConfig config) {
        
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
                config.position,
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::Velocity>(
            entity,
            cmpt::Velocity{ config.direction.x, 0.0f, config.direction.z }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::PROJECTILE_LAYER,
                .mask = data::cnst::PROJECTILE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::BULLET_SIZE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            entity,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::PROJECTILE_LIFETIME
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ config.damage }
        );

        world.AddComponent<cmpt::AppliesKnockback>(
            entity,
            cmpt::AppliesKnockback { 
                .scale = config.knockback_scale, 
                .duration = config.knockback_duration
            }
        );

        world.AddComponent<cmpt::Penetration>(
            entity,
            cmpt::Penetration { config.penetration }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = data::cnst::BULLET_SIZE, 
                .scale = data::cnst::BASE_SCALE,
                .color = data::cnst::BULLET_COLOR,
                .model = &rsrc::asset::bullet_model,
            }
        );
    }
}