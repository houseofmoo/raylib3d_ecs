#include "spawners/world/projectile/explosion.h"

#include "raymath.h"
#include "components/components.h"
#include "resources/asset_loader.h"

namespace spwn::proj {
    void Explosion(strg::Registry& world, const ExplosionConfig config) {
        auto entity = world.CreateEntity();

        world.AddComponent<cmpt::Transform>(
            entity,
            cmpt::Transform{ 
                config.position,
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::PROJECTILE_LAYER,
                .mask = data::cnst::PROJECTILE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = config.start_size
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            entity,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = config.duration
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ 
                .amount = config.damage,
                .penetration = 999
            }
        );

        world.AddComponent<cmpt::AppliesKnockback>(
            entity,
            cmpt::AppliesKnockback { 
                .scale = config.knockback_scale,
                .duration = config.knockback_duration
            }
        );

        world.AddComponent<cmpt::Explosion>(
            entity,
            cmpt::Explosion{
                .duration = config.duration,
                .elapsed = 0.0f,
                .start_size = config.start_size,
                .end_size = config.end_size
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = config.start_size,
                .scale = data::cnst::BASE_SCALE,
                .color = ORANGE,
                .model = &rsrc::asset::grenade_model,
            }
        );
    }
}