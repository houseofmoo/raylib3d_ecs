#include "spawners/world/player/player.h"
#include "raymath.h"
#include "data/entity.h"
#include "assets/assets.h"
#include "components/components.h"

namespace spwn::player {
    Entity Player(strg::Registry& world) {

        auto entity = world.CreateEntity();

        world.AddComponent<cmpt::AttackIntent>(
            entity,
            cmpt::AttackIntent {
                .active = true,
                .from_position = { 0.0f, data::cnst::PLAYER_SIZE.y * 0.5f, 0.0f },
                .to_position = Vector3Zero(),
                .direction = Vector3Zero(),
            }
        );

        world.AddComponent<tag::CameraFollow>(
            entity,
            tag::CameraFollow{}
        );
        
        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::PLAYER_LAYER,
                .mask = data::cnst::PLAYER_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::PLAYER_SIZE
            }
        );

        world.AddComponent<cmpt::DamageReceiver>(
            entity,
            cmpt::DamageReceiver{ 
                .total = 0,
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = data::cnst::PLAYER_SIZE,
                .scale = data::cnst::BASE_SCALE,
                .color = data::cnst::PLAYER_COLOR,
                .model = &asset::models[asset::ModelType::Player]
            }
        );

        world.AddComponent<cmpt::Input>(
            entity,
            cmpt::Input{ 
                .direction = Vector3Zero(), 
                .mouse_world_position = Vector3Zero() 
            }
        );

        world.AddComponent<cmpt::Player>(
            entity,
            cmpt::Player{
                .level = 1,
                .exp = 0,
                .exp_to_level = data::cnst::PLAYER_START_LEVEL_EXP,
                .money = 0,
                .enemies_defeated = 0
            }
        );

        world.AddComponent<cmpt::Stats>(
            entity,
            cmpt::Stats{ 
                .current_hp = data::cnst::PLAYER_START_HP,
                .max_hp = data::cnst::PLAYER_START_HP,
                .move_speed = data::cnst::PLAYER_SPEED,
                // modifiers
                .damage_modifier = 1.0f,
                .attack_speed_modifier = 1.0f,
                .move_speed_modifier = 1.0f,
                .dash_speed_modifier = data::cnst::PLAYER_DASH_RANGE,
                .pickup_range_modifier = 1.0f, 
            }
        );

        world.AddComponent<cmpt::Transform>(
            entity,
            cmpt::Transform{ 
                Vector3{ 0.0f, data::cnst::PLAYER_SIZE.y * 0.5f, 0.0f },
                QuaternionIdentity()
            }
        );
        
        world.AddComponent<cmpt::Velocity>(
            entity,
            cmpt::Velocity{ 0.0f, 0.0f, 0.0f }
        );

        return entity;
    }
}