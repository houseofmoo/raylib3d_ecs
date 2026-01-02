#pragma once

#include "state/state.h"

namespace state {
    struct StartMenu : IState {
        StateKind GetKind() const override { return StateKind::Menu; }
        bool BlocksUpdateBelow() const override { return true; }
        bool BlocksDrawBelow() const override { return true; }
        void HandleInput(StateContext& ctx) override ;
        void Update(StateContext& ctx) override;
        void Draw(StateContext& ctx) override;
    };
}
