#include "spawners/player/player.h"
#include "raymath.h"
#include "data/entity.h"
#include "resources/assets.h"
#include "components/components.h"
#include "components/tags.h"

namespace spwn::player {
    Entity Player(Storage::Registry& world) {

        auto player = world.CreateEntity();
        
        world.AddComponent<tag::Player>(
            player,
            tag::Player{}
        );
        
        world.AddComponent<cmpt::Transform>(
            player,
            cmpt::Transform{ 
                Vector3{ 0.0f, data::size::PLAYER.y * 0.5f, 0.0f },
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
                .layer = data::layer::PLAYER,
                .mask = data::layer::ENEMY | data::layer::LOOT | data::layer::TERRAIN,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = data::size::PLAYER
            }
        );

        world.AddComponent<cmpt::Health>(
            player,
            cmpt::Health{100}
        );

        world.AddComponent<cmpt::DamageReceiver>(
            player,
            cmpt::DamageReceiver{0}
        );

        world.AddComponent<cmpt::Speed>(
            player,
            cmpt::Speed{ 
                .speed = 10.0f, 
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
                .size = data::size::PLAYER,
                .color = BLUE,
                .model = &rsrc::asset::player_model
            }
        );

        return player;
    }
}