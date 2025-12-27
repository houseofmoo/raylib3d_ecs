#pragma once
#include "raylib.h"
#include "raymath.h"
#include "storage./registry.h"
#include "components/components.h"
#include "resources/asset_loader.h"

namespace spwn::test {
    inline void DamageZone(strg::Registry& world) {
        auto entity = world.CreateEntity();
        world.AddComponent<cmpttest::DamageZone>(entity, cmpttest::DamageZone{});

        world.AddComponent<cmpt::Transform>(
            entity,
            cmpt::Transform{ 
                { 0.0f, 3.5f, 0.0f },
                QuaternionIdentity()
            }
        );

        world.AddComponent<cmpt::Collider>(
            entity,
            cmpt::Collider{
                .layer = data::cnst::PROJECTILE_LAYER,
                .mask = data::cnst::PROJECTILE_LAYER_MASK,
                .offset = { 0.0f, 0.0f, 0.0f },
                .size = { 7.0f, 7.0f, 7.0f }
            }
        );

        world.AddComponent<cmpt::DamageDealer>(
            entity,
            cmpt::DamageDealer{ 
                .amount = 1,
            }
        );

        world.AddComponent<cmpt::Draw>(
            entity,
            cmpt::Draw{
                .size = { 7.0f, 7.0f, 7.0f },
                .scale = data::cnst::BASE_SCALE,
                .color = Color{ 0, 0, 0, 100 },
                .model = nullptr
            }
        );
    }
}