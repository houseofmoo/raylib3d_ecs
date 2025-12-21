#pragma once

#include "raylib.h"
#include "storage/registry.h"
#include "components/components.h"

namespace spwn::enemy {
    void Grunt(Storage::Registry& world, 
            const Vector3 position, 
            const cmpt::MoveIntentType move_type, 
            const int hp);
}