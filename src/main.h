#pragma once

#include "raylib.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "boxonbox.h"

#ifdef DEBUG
#include "rlImGui.h"
#include "imgui.h"
#include "debug/debug_window.h"
#include "utils/debug.h"
#endif

#include "data/game/game.h"
#include "assets/assets.h"
#include "assets/asset_loader.h"
#include "systems/systems.h"
#include "components/components.h"

void HandlePause(bool pressed);
void StateSystems(const float delta_time);
void StateDraws(const float delta_time, const int screen_width, const int screen_height);

