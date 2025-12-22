#pragma once

#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib_styles/boxonbox.h"

#ifdef DEBUG
#include "rlImGui.h"
#include "imgui.h"
#include "debug/debug_window.h"
#include "utils/debug.h"
#endif

#include "data/game/game.h"
#include "resources/asset_loader.h"
#include "systems/systems.h"
#include "components/components.h"
#include "draw.h"

