#pragma once

#include <unordered_set>
#include "data/entity.h"

namespace strg {
    // represents an unordered pair of colliding entities
    // order does NOT matter: (A,B) and (B,A) are stored the same (min, max).
    struct CollisionPair {
        Entity entity_a;
        Entity entity_b;

        CollisionPair(Entity x, Entity y) noexcept {
            if (x < y) { 
                entity_a = x; entity_b = y; 
            }
            else { 
                entity_a = y; entity_b = x; 
            }
        }

        bool operator==(const CollisionPair& other) const noexcept {
            return entity_a == other.entity_a && entity_b == other.entity_b;
        }
    };

    struct CollisionPairHash {
        size_t operator()(const CollisionPair& p) const noexcept {
            size_t h1 = std::hash<Entity>{}(p.entity_a);
            size_t h2 = std::hash<Entity>{}(p.entity_b);
            // boost::hash_combine style
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ull + (h1 << 6) + (h1 >> 2));
        }
    };

    struct CollisionEnterCache {
        std::unordered_set<CollisionPair, CollisionPairHash> previous;
        std::unordered_set<CollisionPair, CollisionPairHash> current;

        bool IsEnter(const CollisionPair& p) const {
            return !previous.contains(p) && current.contains(p);
        }

        bool IsStay(const CollisionPair& p) const {
            return previous.contains(p) && current.contains(p);
        }

        bool IsExit(const CollisionPair& p) const {
            return previous.contains(p) && !current.contains(p);
        }
    };
}
