#pragma once

#include "raylib.h"
#include "raymath.h"
#include "data/entity.h"
#include "components/components.h"
#include "storage/registry.h"

namespace utils {
    inline float GetEntityHeight(const Vector3 position, const Vector3 collider_size) {
        return position.y - (collider_size.y * 0.5f);
    }

    inline float GetEntityHeight(strg::Registry& world, const Entity id) {
        auto* trans = world.TryGetComponent<cmpt::Transform>(id);
        if (trans == nullptr) {
            return 0.0f;
        }

        auto* col = world.TryGetComponent<cmpt::Collider>(id);
        if (col == nullptr) {
            return 0.0f;
        }

        return GetEntityHeight(trans->position, col->size);
    }

    inline BoundingBox GetBoundingBox(const Vector3 position, const Vector3 offset, const Vector3 size) {
        return BoundingBox{
            (position + offset) - (size * 0.5f),
            (position + offset) + (size * 0.5f)
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

    inline Quaternion GetRotationToLocation(Vector3 source, Vector3 location) {
        Vector3 dir = location - source;
        dir.y = 0.0f;

        if (Vector3LengthSqr(dir) > 0.0001f) {
            float angle = -(atan2f(dir.z, dir.x) - PI/2.0f);
            return QuaternionFromAxisAngle({ 0.0f, 1.0f, 0.0f}, angle);
        }
        return QuaternionIdentity();
    }

    inline Quaternion GetRotationToDirection(Vector3 direction) {
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

    // Remove Y component then normalize
    inline Vector3 FlattenThenNormalize(Vector3 vec) noexcept {
        vec.y = 0.0f;
        return Vector3Normalize(vec);
    }

    // Get the direction from ME to THEM
    inline Vector3 Direction(const Vector3& me, const Vector3& them) noexcept {
        return them - me;
    }

    // Get the direction from ME to THEM then normalize
    inline Vector3 DirectionNormalized(const Vector3& me, const Vector3& them) noexcept {
        return Vector3Normalize(Direction(me, them));
    }

    // Get the direction from ME to THEM, then remove Y component and normalize
    inline Vector3 DirectionFlattenThenNormalize(const Vector3& me, const Vector3& them) noexcept {
        return FlattenThenNormalize(Direction(me, them));
    }

    inline float EaseInOutQuad(float t) {
        if (t < 0.5f) {
            return 2.0f * t * t;
        }
        return 1.0f - powf(-2.0f * t + 2.0f, 2.0f) * 0.5f;
    }

    inline float Vector3Area(Vector3 vec) {
        return vec.x*vec.y*vec.z;
    }
}