#pragma once

#include "storage/registry.h"

namespace sys::col {
    void DamageOnCollision(Storage::Registry& world);
    void DestroyOnCollision(Storage::Registry& world);
    void KnockbackOnCollision(Storage::Registry& world);
    void PickupOnCollision(Storage::Registry& world);
    void TriggerOnCollision(Storage::Registry& world);
    void RepositionOnCollision(Storage::Registry& world);
}