#pragma once

#include "imgui.h"
#include "storage/registry.h"

namespace debug {
    void DrawDebugUI(strg::Registry& world, ImGuiIO& io);
}
