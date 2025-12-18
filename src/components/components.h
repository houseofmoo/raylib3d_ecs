#pragma once

#include <array>
#include <cinttypes>

#include "raylib.h"

#include "data/entity.h"
#include "components/stats.h"
#include "components/loot.h"
#include "components/weapon.h"
#include "components/notifications.h"
#include "components/status_effects.h"
#include "components/animations.h"

namespace cmpt {
    struct Transform {
        Vector3 position;
        Quaternion rotation;
    };

    // normal horizontal movement
    typedef Vector3 Velocity; 

    // things that move in an arch (grenades, other throwables ect)
    struct ArchMove { 
        Vector3 start;
        Vector3 end;
        float duration;
        float elapsed;
        float height;
    };

    // rotates in place
    struct RotateInPlace { 
        float speed;
    };

    enum class MoveIntentType {
        Seek,
        Random,
    };

    // non-player characters have this value for AI movement
    struct MoveIntent { 
        MoveIntentType type;
        Vector3 direction;
    };

    struct Input {
        Vector3 direction;
        Vector3 mouse_world_position;
    };
    
    struct Collider {
        Layer layer;
        Mask mask;
        Vector3 offset;
        Vector3 size;
    };
    
    struct DestroyOnContact {
        int penetration;
    };

    struct Lifetime {
        double start_time;
        double countdown;
    };

    struct Draw {
        Vector3 size;
        Color color;
        Model* model;
    };
}