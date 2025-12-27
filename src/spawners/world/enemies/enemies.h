#pragma once

#include "raylib.h"
#include "data/entity.h"
#include "storage/registry.h"
#include "components/components.h"

namespace spwn::enemy {
    Entity Grunt(
        strg::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp
    );

    Entity Brute(
        strg::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp
    );
}