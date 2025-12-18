#pragma once

#include <cinttypes>
#include "raylib.h"

using Entity = uint32_t;
using Layer = uint32_t;
using Mask = uint32_t;

namespace data {
    
    // layer masks
    namespace layer {
        constexpr Layer PLAYER     = 1 << 0;
        constexpr Layer ENEMY      = 1 << 1;
        constexpr Layer PROJECTILE = 1 << 2;
        constexpr Layer LOOT       = 1 << 3;
        constexpr Layer TERRAIN    = 1 << 4;
        // Layer TERRAIN    = 1 << 5;
        // Layer TERRAIN    = 1 << 6;
        // Layer TERRAIN    = 1 << 7;
        // Layer TERRAIN    = 1 << 8;
        // Layer TERRAIN    = 1 << 9;

        inline bool Interacts(Layer layer_a, Mask mask_a, Layer layer_b, Mask mask_b) {
            return (layer_a & mask_b) > 0 || (layer_b & mask_a) > 0;
        }
    }

    // temp sizing info
    namespace size {
        constexpr Vector3 PLAYER = { 1.0f, 2.0f, 1.0f };
        constexpr Vector3 PROJECTILE = { 0.25f, 0.25f, 0.25f };
        constexpr Vector3 ENEMY = {1.0f, 1.5f, 1.0f};
        constexpr Vector3 LOOT = {0.5f, 0.5f, 0.5f};
    }
}