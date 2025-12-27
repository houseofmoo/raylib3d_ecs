#pragma once

#include "raylib.h"
#include "storage/registry.h"
#include "components/components.h"

namespace spwn::enemy {
    void Grunt(
        strg::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp
    );

    void Brute(
        strg::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp
    );
}