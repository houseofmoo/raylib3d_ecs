#include "spawners/world/player/player.h"
#include "raymath.h"
#include "data/entity.h"
#include "resources/asset_loader.h"
#include "components/components.h"

namespace spwn::player {
    Entity Player(Storage::Registry& world) {

        auto player = world.CreateEntity();
        
        world.AddComponent<tag::Player>(
            player,
            tag::Player{}
        );

        world.AddComponent<tag::CameraFollow>(
            player,
            tag::CameraFollow{}
        );
        
        world.AddComponent<cmpt::Transform>(
            player,
            cmpt::Transform{ 
                Vector3{ 0.0f, data::cnst::PLAYER_SIZE.y * 0.5f, 0.0f },
                QuaternionIdentity()
            }
        );
        
        world.AddComponent<cmpt::Velocity>(
            player,
            cmpt::Velocity{ 0.0f, 0.0f, 0.0f }
        );

        world.AddComponent<cmpt::Collider>(
            player,
            cmpt::Collider{
                .layer = data::cnst::PLAYER_LAYER,
                .mask = data::cnst::PLAYER_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::cnst::PLAYER_SIZE
            }
        );

        world.AddComponent<cmpt::Health>(
            player,
            cmpt::Health{ data::cnst::PLAYER_START_HP, data::cnst::PLAYER_START_HP }
        );

        world.AddComponent<cmpt::DamageReceiver>(
            player,
            cmpt::DamageReceiver{0}
        );

        world.AddComponent<cmpt::Speed>(
            player,
            cmpt::Speed{ 
                .speed = data::cnst::PLAYER_SPEED, 
                .speed_multiplier = 1.0f, 
                .dash_multiplier = 1.0f,
            }
        );

        world.AddComponent<cmpt::Input>(
            player,
            cmpt::Input{ 
                .direction = Vector3Zero(), 
                .mouse_world_position = Vector3Zero() 
            }
        );

        world.AddComponent<cmpt::Draw>(
            player,
            cmpt::Draw{ 
                .size = data::cnst::PLAYER_SIZE,
                .color = data::cnst::PLAYER_COLOR,
                .model = &rsrc::asset::player_model
            }
        );

        return player;
    }
}