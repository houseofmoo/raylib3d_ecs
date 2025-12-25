#include "spawners/world/projectile/explosion.h"

#include "raymath.h"
#include "components/components.h"
#include "resources/asset_loader.h"

namespace spwn::proj {
    void Explosion(Storage::Registry& world, const Vector3 position, const int damage) {
        auto entity = world.CreateEntity();

        world.AddComponent<cmpt::Transform>(
            entity,
            cmpt::Transform{ 
                position,
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::PROJECTILE_LAYER,
                .mask = data::cnst::PROJECTILE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::EXPLOSION_START_SIZE
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            entity,
            cmpt::Lifetime{ 
                .start_time = GetTime(),
                .countdown = data::cnst::EXPLOSION_DURATION
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ 
                .amount = damage,
                .penetration = 999
            }
        );

        world.AddComponent<cmpt::AppliesKnockback>(
            entity,
            cmpt::AppliesKnockback { 
                .scale = data::cnst::EXPLOSION_KNOCKBACK_SCALE, 
                .duration = data::cnst::EXPLOSION_KNOCKBACK_DURATION
            }
        );

        world.AddComponent<cmpt::Explosion>(
            entity,
            cmpt::Explosion{
                .duration = data::cnst::EXPLOSION_DURATION,
                .elapsed = 0.0f,
                .start_size = data::cnst::EXPLOSION_START_SIZE,
                .end_size = data::cnst::EXPLOSION_END_SIZE
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = data::cnst::EXPLOSION_START_SIZE, 
                .scale  = data::cnst::BASE_SCALE,
                .color = ORANGE,
                .model = &rsrc::asset::grenade_model,
            }
        );
    }
}