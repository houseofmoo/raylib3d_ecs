#include "spawners/world/enemies/enemies.h"

#include "raymath.h"
#include "resources/asset_loader.h"
#include "components/cmpt_helpers.h"

namespace spwn::enemy {
    Entity Brute(
            strg::Registry& world, 
            const Vector3 position, 
            const cmpt::AIMoveMode move_mode, 
            const int hp) {

        auto entity = world.CreateEntity();

        // for drop in animation
        Vector3 start_position = Vector3{ position.x, 50.0f, position.z };
        Vector3 end_position = Vector3{ position.x, data::cnst::BRUTE_SIZE.y * 0.5f, position.z };
        
        world.AddComponent<tag::Enemy>(
            entity,
            tag::Enemy{}
        );

        cmpt::AttachAIMovementComponent(world, entity, move_mode);
        world.AddComponent<cmpt::AIMoveIntent>(
            entity,
            cmpt::AIMoveIntent{
                .mode = move_mode,
                .position = Vector3Zero(),
                .direction = Vector3Zero(),
                .start_rotation = QuaternionIdentity(),
                .rotation_complete = true,
                .rotation_duration = data::cnst::ENEMY_ROTATION_DURATION,
                .rotation_elapsed = 0.0f,
                .stuck = false,
            }
        );

        world.AddComponent<cmpt::AttackIntent>(
            entity,
            cmpt::AttackIntent {
                .active = false,
                .from_position = start_position,
                .to_position = Vector3Zero(),
                .direction = Vector3Zero(),
            }
        );

        world.AddComponent<cmpt::AppliesKnockback>(
            entity,
            cmpt::AppliesKnockback{
                .scale = data::cnst::BRUTE_KNOCKBACK_SCALE,
                .duration = data::cnst::BRUTE_KNOCKBACK_DURATION
            }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::BRUTE_LAYER,
                .mask = data::cnst::BRUTE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::BRUTE_SIZE
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ data::cnst::BRUTE_MELEE_DMG }
        );

        world.AddComponent<cmpt::DamageReceiver>(
            entity,
            cmpt::DamageReceiver{ 0 }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = data::cnst::BRUTE_SIZE,
                .scale = data::cnst::BASE_SCALE,
                .color = data::cnst::BRUTE_COLOR, 
                .model = &rsrc::asset::brute_model,
            }
        );

        world.AddComponent<cmpt::DropsLoot>(
            entity,
            cmpt::DropsLoot{ data::cnst::BRUTE_LOOT_MULTIPLIER }
        );

        world.AddComponent<cmpt::SpawnAnimation>(
            entity,
            cmpt::SpawnAnimation{
                .start_position = start_position,
                .end_position = end_position
            }
        );

        world.AddComponent<cmpt::Stats>(
            entity,
            cmpt::Stats{ 
                .current_hp = hp,
                .max_hp = hp,
                .move_speed = data::cnst::BRUTE_SPEED,
                // modifiers
                .damage_modifier = 1.0f,
                .attack_speed_modifier = 1.0f,
                .move_speed_modifier = 1.0f,
                .dash_speed_modifier = 1.0f,
                .pickup_range_modifier = 1.0f, 
            }
        );

        world.AddComponent<cmpt::Transform>(
            entity,
            cmpt::Transform{ 
                .position = start_position,
                .rotation = QuaternionIdentity()
            }
        );
        
        world.AddComponent<cmpt::Velocity>(
            entity,
            cmpt::Velocity{ 0.0f, 0.0f, 0.0f }
        );

        return entity;
    }
}