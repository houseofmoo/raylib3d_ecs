#include "spawners/terrain/terrain.h"
#include "raymath.h"
#include "components/components.h"
#include "components/tags.h"

namespace spwn::terrain {
    void Terrain(Storage::Registry& world, const Vector3 position, const Vector3 size, const bool visible) {
        auto terrain = world.CreateEntity();
        
        world.AddComponent<tag::Terrain>(
            terrain,
            tag::Terrain{}
        );

        world.AddComponent<cmpt::Transform>(
            terrain,
            cmpt::Transform{
                Vector3 { position.x, size.y * 0.5f, position.z },
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::Collider>(
            terrain,
            cmpt::Collider{
                .layer = data::layer::TERRAIN,
                .mask = data::layer::PLAYER | data::layer::ENEMY | data::layer::PROJECTILE,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = size
            }
        );

        if (visible) {
            world.AddComponent<cmpt::Draw>(
                terrain,
                cmpt::Draw{ 
                    size, 
                    DARKGRAY, 
                    nullptr
                }
            );
        }
    }
}