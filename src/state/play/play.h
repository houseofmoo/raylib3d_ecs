#pragma once

#include "state/state.h"

namespace state {
    struct Play : IState {
        StateKind GetKind() const override { return StateKind::Gameplay; }
        bool BlocksUpdateBelow() const override { return false; }
        bool BlocksDrawBelow() const override { return false; }
        void OnEnter() override;
        void HandleInput(StateContext& ctx) override;
        void Update(StateContext& ctx) override;
        void Draw(StateContext& ctx) override;
    };
}
