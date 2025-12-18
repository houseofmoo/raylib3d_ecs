
#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "components/tags.h"

namespace sys {
    inline void ConstraintToWorld(Storage::Registry& world, const BoundingBox world_boundary) {
        for (auto entity : world.View<cmpt::Transform, cmpt::Velocity>()) {
            auto& trans = world.GetComponent<cmpt::Transform>(entity);

            // if position ends up out side bounday, move them backin
            if (trans.position.x < world_boundary.min.x) {
                trans.position.x = world_boundary.min.x;
            } else if (trans.position.x > world_boundary.max.x) {
                trans.position.x = world_boundary.max.x;
            }

            if (trans.position.z < world_boundary.min.z) {
                trans.position.z = world_boundary.min.z;
            } else if (trans.position.z > world_boundary.max.z) {
                trans.position.z = world_boundary.max.z;
            }

            if (trans.position.y <= world_boundary.min.y) {
                if (world.HasComponent<cmpt::Collider>(entity)) {
                    auto& collider = world.GetComponent<cmpt::Collider>(entity);
                    trans.position.y = world_boundary.min.y + (collider.size.y * 0.5f);
                } else {
                    trans.position.y = world_boundary.min.y;
                }
            } else if (trans.position.y >= world_boundary.max.y) {
                trans.position.y = world_boundary.max.y;
            }
        }
    }
}