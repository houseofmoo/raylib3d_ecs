#pragma once

#include "raylib.h"
#include "raymath.h"
#include "components/components.h"

namespace utils {
    inline BoundingBox GetBoundingBox(const Vector3 position, const Vector3 offset, const Vector3 size) {
        Vector3 center = Vector3Add(position, offset);
        Vector3 half_size =  size * 0.5f;

        return BoundingBox{
            Vector3Subtract(center, half_size),
            Vector3Add(center, half_size)
        };
    }

    inline BoundingBox GetBoundingBox(const Vector3 position, const Vector3 size) {
        return GetBoundingBox(position, Vector3Zero(), size);
    }

    inline BoundingBox GetBoundingBox(const cmpt::Transform& trans, const cmpt::Collider col) {
        return GetBoundingBox(trans.position, col.offset, col.size);
    }

    inline bool PointInsideBox(const Vector3 p, const BoundingBox b) {
        return (p.x >= b.min.x && p.x <= b.max.x) &&
            (p.y >= b.min.y && p.y <= b.max.y) &&
            (p.z >= b.min.z && p.z <= b.max.z);
    }

    inline Vector3 PushbackDirection(const BoundingBox& a, const BoundingBox& b) {
        // get direction box a should be pushed back so it no longer
        // overlaps with box b
        
        // assumes they intersect
        float axCenter = (a.min.x + a.max.x) * 0.5f;
        float ayCenter = (a.min.y + a.max.y) * 0.5f;
        float azCenter = (a.min.z + a.max.z) * 0.5f;

        float bxCenter = (b.min.x + b.max.x) * 0.5f;
        float byCenter = (b.min.y + b.max.y) * 0.5f;
        float bzCenter = (b.min.z + b.max.z) * 0.5f;

        float dx = axCenter - bxCenter;
        float dy = ayCenter - byCenter;
        float dz = azCenter - bzCenter;

        float axHalf = (a.max.x - a.min.x) * 0.5f;
        float ayHalf = (a.max.y - a.min.y) * 0.5f;
        float azHalf = (a.max.z - a.min.z) * 0.5f;

        float bxHalf = (b.max.x - b.min.x) * 0.5f;
        float byHalf = (b.max.y - b.min.y) * 0.5f;
        float bzHalf = (b.max.z - b.min.z) * 0.5f;

        float ox = (axHalf + bxHalf) - fabsf(dx);
        float oy = (ayHalf + byHalf) - fabsf(dy);
        float oz = (azHalf + bzHalf) - fabsf(dz);

        // push out along smallest overlap axis
        if (ox < oy && ox < oz) {
            return { (dx < 0.0f ? -ox : ox), 0.0f, 0.0f };
        } else if (oy < oz) {
            return { 0.0f, (dy < 0.0f ? -oy : oy), 0.0f };
        } else {
            return { 0.0f, 0.0f, (dz < 0.0f ? -oz : oz) };
        }
    }

    inline Quaternion GetRotationToLocation(Vector3 source, Vector3 location) {
        Vector3 dir =  Vector3Subtract(location, source);
        dir.y = 0.0f;

        if (Vector3LengthSqr(dir) > 0.0001f) {
            float angle = -(atan2f(dir.z, dir.x) - PI/2.0f);
            return QuaternionFromAxisAngle({ 0.0f, 1.0f, 0.0f}, angle);
        }
        return QuaternionIdentity();
    }

    inline Quaternion GetRotationToDirection(Vector3 source, Vector3 direction) {
        direction.y = 0.0f;
        if (Vector3LengthSqr(direction) > 0.0001f) {
            float angle = -(atan2f(direction.z, direction.x) - PI/2.0f);
            return QuaternionFromAxisAngle({ 0.0f, 1.0f, 0.0f}, angle);
        }
        return QuaternionIdentity();
    }

    inline Vector3 RotateDirYaw(Vector3 dir, float radians) {
        // assumes dir is normalized-ish
        float c = cosf(radians);
        float s = sinf(radians);

        return {
            dir.x * c - dir.z * s,
            dir.y,
            dir.x * s + dir.z * c
        };
    }

    inline Vector3 FlattenAndNormalize(Vector3 vec) noexcept {
        vec.y = 0.0f;
        return Vector3Normalize(vec);
    }

    inline Vector3 Direction(const Vector3& me, const Vector3& them) noexcept {
        return Vector3Subtract(them, me);
    }
}