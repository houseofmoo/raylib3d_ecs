#pragma once

#include "imgui.h"
#include "storage/registry.h"

namespace debug {
    void DrawDebugUI(Storage::Registry& world, ImGuiIO& io);
}
