#include "spawners/world/projectile/explosion.h"

#include "raymath.h"
#include "components/components.h"
#include "assets/assets.h"

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
                .layer = config.layer,
                .mask = config.mask,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = config.start_size
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            entity,
            cmpt::Lifetime{ 
                .countdown = config.duration
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ 
                .amount = config.damage,
            }
        );

        world.AddComponent<cmpt::AppliesKnockback>(
            entity,
            cmpt::AppliesKnockback { 
                .scale = config.knockback_scale,
                .duration = config.knockback_duration
            }
        );
        
        world.AddComponent<cmpt::LargeAoE>(
            entity,
            cmpt::LargeAoE{}
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
                .model = &asset::models[asset::ModelType::Grenade],
            }
        );
    }
}