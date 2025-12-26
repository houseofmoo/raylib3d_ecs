#pragma once

#include "storage/registry.h"

namespace sys::col {
    void DamageOnCollision(strg::Registry& world);
    void DestroyOnCollision(strg::Registry& world);
    void KnockbackOnCollision(strg::Registry& world);
    void PickupOnCollision(strg::Registry& world);
    void TriggerOnCollision(strg::Registry& world);
    void RepositionOnCollision(strg::Registry& world);
}