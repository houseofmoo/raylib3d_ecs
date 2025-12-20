#pragma once

#include "raylib.h"
#include "data/game/game.h"

namespace utils {
    inline void MoveAndSlideTerrain(Vector3& pos, Vector3 delta) {
        Vector3 tryX = pos;
        tryX.x += delta.x;
        if (!data::game::terrain.IsBlockedWorld(tryX.x, tryX.z)) pos.x = tryX.x;

        Vector3 tryZ = pos;
        tryZ.z += delta.z;
        if (!data::game::terrain.IsBlockedWorld(tryZ.x, tryZ.z)) pos.z = tryZ.z;
    }

    // Minimum Translation Vector
    inline Vector3 PushbackMTV_XZ(const BoundingBox& a, const BoundingBox& b) {
        // centers
        float ax = (a.min.x + a.max.x) * 0.5f;
        float az = (a.min.z + a.max.z) * 0.5f;
        float bx = (b.min.x + b.max.x) * 0.5f;
        float bz = (b.min.z + b.max.z) * 0.5f;

        float dx = ax - bx;
        float dz = az - bz;

        // half extents
        float ahx = (a.max.x - a.min.x) * 0.5f;
        float ahz = (a.max.z - a.min.z) * 0.5f;
        float bhx = (b.max.x - b.min.x) * 0.5f;
        float bhz = (b.max.z - b.min.z) * 0.5f;

        float ox = (ahx + bhx) - std::fabs(dx);
        float oz = (ahz + bhz) - std::fabs(dz);

        // If not overlapping (safety)
        if (ox <= 0.0f || oz <= 0.0f) return {0,0,0};

        // push out along smallest overlap axis (XZ only)
        if (ox < oz) {
            return { (dx < 0.0f ? -ox : ox), 0.0f, 0.0f };
        } else {
            return { 0.0f, 0.0f, (dz < 0.0f ? -oz : oz) };
        }
    }

    // inline Vector3 PushbackDirection(const BoundingBox& a, const BoundingBox& b) {
    //     // get direction box a should be pushed back so it no longer
    //     // overlaps with box b
        
    //     // assumes they intersect
    //     float axCenter = (a.min.x + a.max.x) * 0.5f;
    //     float ayCenter = (a.min.y + a.max.y) * 0.5f;
    //     float azCenter = (a.min.z + a.max.z) * 0.5f;

    //     float bxCenter = (b.min.x + b.max.x) * 0.5f;
    //     float byCenter = (b.min.y + b.max.y) * 0.5f;
    //     float bzCenter = (b.min.z + b.max.z) * 0.5f;

    //     float dx = axCenter - bxCenter;
    //     float dy = ayCenter - byCenter;
    //     float dz = azCenter - bzCenter;

    //     float axHalf = (a.max.x - a.min.x) * 0.5f;
    //     float ayHalf = (a.max.y - a.min.y) * 0.5f;
    //     float azHalf = (a.max.z - a.min.z) * 0.5f;

    //     float bxHalf = (b.max.x - b.min.x) * 0.5f;
    //     float byHalf = (b.max.y - b.min.y) * 0.5f;
    //     float bzHalf = (b.max.z - b.min.z) * 0.5f;

    //     float ox = (axHalf + bxHalf) - fabsf(dx);
    //     float oy = (ayHalf + byHalf) - fabsf(dy);
    //     float oz = (azHalf + bzHalf) - fabsf(dz);

    //     // push out along smallest overlap axis
    //     if (ox < oy && ox < oz) {
    //         return { (dx < 0.0f ? -ox : ox), 0.0f, 0.0f };
    //     } else if (oy < oz) {
    //         return { 0.0f, (dy < 0.0f ? -oy : oy), 0.0f };
    //     } else {
    //         return { 0.0f, 0.0f, (dz < 0.0f ? -oz : oz) };
    //     }
    // }
}