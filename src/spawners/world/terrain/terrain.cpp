#include "spawners/world/terrain/terrain.h"
#include "raymath.h"
#include "components/components.h"
#include "data/game/game.h"

namespace spwn::terrain {
     void Terrain(strg::Registry& world, const Vector3 position, const Vector3 size, const bool visible) {
        auto terrain = world.CreateEntity();
        
        // world.AddComponent<tag::Terrain>(
        //     terrain,
        //     tag::Terrain{}
        // );

        world.AddComponent<cmpt::Transform>(
            terrain,
            cmpt::Transform{
                .position = position, //Vector3 { position.x, size.y * 0.5f, position.z },
                .rotation = QuaternionIdentity()
            }
        );

        if (visible) {
            world.AddComponent<cmpt::Draw>(
                terrain,
                cmpt::Draw{ 
                    .size = size,
                    .scale = data::cnst::BASE_SCALE, 
                    .color = DARKGRAY, 
                    .model = nullptr
                }
            );
        }

        // add to terrain tilemap
        data::g_terrain.SetAreaTiles(position, size);
    }
}