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
        //constexpr Layer TERRAIN    = 1 << 4;
        //constexpr Layer LAYER_TBD  = 1 << 5;
        //constexpr Layer LAYER_TBD  = 1 << 6;
        //constexpr Layer LAYER_TBD  = 1 << 7;
        //constexpr Layer LAYER_TBD  = 1 << 8;
        //constexpr Layer LAYER_TBD  = 1 << 9;

        inline bool InteractsOneWay(Layer layer_a, Mask mask_a, Layer layer_b, Mask mask_b) {
            // if either interact with the other
            return (layer_a & mask_b) > 0 || (layer_b & mask_a) > 0;
        }

        inline bool InteractsBoth(Layer layer_a, Mask mask_a, Layer layer_b, Mask mask_b) {
            // must interact with each other
            return (layer_a & mask_b) > 0 && (layer_b & mask_a) > 0;
        }
    }

    // temp sizing info
    namespace size {
        constexpr Vector3 PLAYER = { 1.0f, 2.0f, 1.0f };
        constexpr Vector3 PROJECTILE = { 0.25f, 0.25f, 0.25f };
        constexpr Vector3 GRUNT = {1.0f, 1.5f, 1.0f};
        constexpr Vector3 BRUTE = {1.5f, 2.5f, 1.5f};
        constexpr Vector3 EXP = {0.3f, 0.3f, 0.3f};
        constexpr Vector3 MONEY = {0.3f, 0.3f, 0.3f};
        constexpr Vector3 POWERUP = {0.5f, 0.5f, 0.5f};
        constexpr Vector3 WEAPON = {1.25f, 0.35f, 0.35f};

        constexpr BoundingBox PLAY_AREA = {
            .min = Vector3{-60.0f, 0.0f, -30.0f},
            .max = Vector3{60.0f, 55.0f, 30.0f}
        };

        inline Vector3 MinColldierSize(Vector3 vec) {
            // min collider size
            if (vec.x * vec.y * vec.z < 0.125f) {
                return Vector3{0.5f, 0.5f, 0.5f};
            }
            return vec;
        }
    }
}