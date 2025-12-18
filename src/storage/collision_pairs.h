#pragma once

#include <unordered_set>
#include "data/entity.h"

namespace strg {
    // Represents a pair of colliding entities
    // Order matters: colA collides with colB
    struct CollisionPair {
        Entity entity_a;
        Entity entity_b;

        CollisionPair(Entity x, Entity y) {
            if (x < y) { entity_a = x; entity_b = y; }
            else { entity_a = y; entity_b = x; }
        }

        bool operator==(const CollisionPair& other) const noexcept {
            return entity_a == other.entity_a && entity_b == other.entity_b;
        }
    };

    struct CollisionPairHash {
        size_t operator()(const CollisionPair& p) const noexcept {
            size_t h1 = std::hash<Entity>{}(p.entity_a);
            size_t h2 = std::hash<Entity>{}(p.entity_a);
            return h1 ^ (h2 << 1);
        }
    };

    struct CollisionEnterCache {
        std::unordered_set<CollisionPair, CollisionPairHash> previous;
        std::unordered_set<CollisionPair, CollisionPairHash> current;
    };
}
