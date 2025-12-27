#pragma once

#include "raylib.h"
#include "raymath.h"
#include "storage/registry.h"
#include "data/game/game.h"
#include "data/entity.h"
#include "utils/debug.h"

namespace utils {
    inline Vector3 GetRandomValidPosition() {
        // assumes looking for a position that is valid at 0.0f height
        float new_x = 0.0f;
        float new_z = 0.0f;
        int attempts = 0;
        while (true) {
            new_x = (float)GetRandomValue(
                data::cnst::PLAY_AREA.min.x + 1.0f, 
                data::cnst::PLAY_AREA.max.x - 1.0f
            );
            new_z = (float)GetRandomValue(
                data::cnst::PLAY_AREA.min.z + 1.0f, 
                data::cnst::PLAY_AREA.max.z - 1.0f
            );
            
            if (data::g_terrain.ValidMove(new_x, new_z, 0.0f)) break;

            attempts += 1;
            if (attempts > 5) {
                PRINT("took more than 5 attempts to GetRandomValidPosition()");
                return Vector3{ 0.0f, 0.0f, 0.0f };
            }
        }

        return Vector3{new_x, 0.0f, new_z};
    }

    inline Vector3 GetRandomValidPositionNearTarget(const Vector3 target, const int offset) {
        // assumes looking for a position that is valid at 0.0f height
        float new_x = 0.0f;
        float new_z = 0.0f;
        int attempts = 0;
        while (true) {
            new_x = (float)GetRandomValue(
                (int)std::floor(target.x - offset), 
                (int)std::ceil(target.x + offset)
            );
            new_z = (float)GetRandomValue(
                (int)std::floor(target.z - offset), 
                (int)std::ceil(target.z + offset)
            );

            if (data::g_terrain.ValidMove(new_x, new_z, 0.0f)) break;
            
            attempts += 1;
            if (attempts > 5) {
                PRINT("took more than 5 attempts to GetRandomValidPisitionNearTarget()");
                return GetRandomValidPosition(); // failed, get global random position
            }
        }

        return Vector3{new_x, 0.0f, new_z};
    }

    inline Vector3 ValidateMovePosition(const Vector3 from_position, const Vector3 to_position, const float height) {
        // if to_position is valid, just return it
        if (data::g_terrain.ValidMove(to_position, height)){
            return to_position;
        }
        
        // check x is valid position
        Vector3 valid_position = from_position;
        
        // if X is not valid, restore from_position.x
        valid_position.x = to_position.x;
        if (!data::g_terrain.ValidMove(valid_position.x, valid_position.z, height)) {
            valid_position.x = from_position.x;
        }

        // if Z is not valid, restore from_position.z
        valid_position.z = to_position.z;
        if (!data::g_terrain.ValidMove(valid_position.x, valid_position.z, height)) {
            valid_position.z = from_position.z;
        }

        return valid_position;
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
}