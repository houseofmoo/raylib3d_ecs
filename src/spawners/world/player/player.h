#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::player {
    Entity Player(strg::Registry& world);
}