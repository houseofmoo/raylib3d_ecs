#pragma once

#include <memory>
#include "raylib.h"
#include "storage/registry.h"
#include "utils/debug.h"

namespace state {
    // Forward decls
    struct StateContext;
    struct IState;

    enum class StateKind {
        Menu,
        Gameplay
    };

    struct IState {
        virtual ~IState() = default;
        virtual void OnEnter() {}
        virtual void OnExit() {}
        
        virtual StateKind GetKind() const = 0;
        virtual bool BlocksUpdateBelow() const = 0;
        virtual bool BlocksDrawBelow() const = 0;
        virtual void HandleInput(StateContext& ctx) = 0;
        virtual void Update(StateContext& ctx) = 0;
        virtual void Draw(StateContext& ctx) = 0;
    };


    struct StateCommands {
        virtual ~StateCommands() = default;
        virtual void Push(std::unique_ptr<IState> s) = 0;
        virtual void Pop() = 0;
        virtual void Replace(std::unique_ptr<IState> s) = 0; // pop then push
        virtual void Clear() = 0;
    };

    struct Input {
        float up;
        float down;
        float left;
        float right;
        bool dash;
        bool pause;
        bool exit;

        void Reset() {
            up = 0.0f;
            down = 0.0f;
            left = 0.0f;
            right = 0.0f;
            dash = false;
            pause = false;
            exit = false;
        }
    };

    struct StateContext {
        StateCommands& cmd;
        strg::Registry& world;
        Camera3D& camera;
        Input input;
        float delta_time;
        // Assets& assets;
        // Audio& audio;
    };
}