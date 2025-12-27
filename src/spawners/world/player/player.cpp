#include "spawners/world/player/player.h"
#include "raymath.h"
#include "data/entity.h"
#include "resources/asset_loader.h"
#include "components/components.h"

namespace spwn::player {
    Entity Player(strg::Registry& world) {

        auto entity = world.CreateEntity();
        
        world.AddComponent<tag::Player>(
            entity,
            tag::Player{}
        );

        world.AddComponent<tag::CameraFollow>(
            entity,
            tag::CameraFollow{}
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

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::PLAYER_LAYER,
                .mask = data::cnst::PLAYER_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::PLAYER_SIZE
            }
        );

        world.AddComponent<cmpt::Health>(
            entity,
            cmpt::Health{ data::cnst::PLAYER_START_HP, data::cnst::PLAYER_START_HP }
        );

        world.AddComponent<cmpt::AttackIntent>(
            entity,
            cmpt::AttackIntent {
                .active = true,
                .from_position = { 0.0f, data::cnst::PLAYER_SIZE.y * 0.5f, 0.0f },
                .to_position = Vector3Zero(),
                .direction = Vector3Zero(),
            }
        );

        world.AddComponent<cmpt::DamageReceiver>(
            entity,
            cmpt::DamageReceiver{0}
        );

        world.AddComponent<cmpt::Speed>(
            entity,
            cmpt::Speed{ 
                .speed = data::cnst::PLAYER_SPEED, 
                .speed_multiplier = 1.0f, 
                .dash_multiplier = 1.0f,
            }
        );

        world.AddComponent<cmpt::Input>(
            entity,
            cmpt::Input{ 
                .direction = Vector3Zero(), 
                .mouse_world_position = Vector3Zero() 
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{ 
                .size = data::cnst::PLAYER_SIZE,
                .scale = data::cnst::BASE_SCALE,
                .color = data::cnst::PLAYER_COLOR,
                .model = &rsrc::asset::player_model
            }
        );

        return entity;
    }
}