#pragma once

#include <utility>
#include "raylib.h"

namespace guard {
    template <auto BeginFn, auto EndFn>
    struct ScopeGuard {
        template <typename... Args>
        explicit ScopeGuard(Args&&... args) noexcept {
            BeginFn(std::forward<Args>(args)...);
        }

        ~ScopeGuard() noexcept  {
            EndFn();
        }

        ScopeGuard(const ScopeGuard&) = delete;
        ScopeGuard& operator=(const ScopeGuard&) = delete;
        ScopeGuard(ScopeGuard&&) = delete;
        ScopeGuard& operator=(ScopeGuard&&) = delete;
    };

    using Drawing = ScopeGuard<::BeginDrawing,     ::EndDrawing>;
    using Mode3D  = ScopeGuard<::BeginMode3D,      ::EndMode3D>;
    using Shader  = ScopeGuard<::BeginShaderMode,  ::EndShaderMode>;
    using Texture = ScopeGuard<::BeginTextureMode, ::EndTextureMode>;
    using Blend   = ScopeGuard<::BeginBlendMode,   ::EndBlendMode>;

    // defer function execution to end of scope
    template <class F>
    struct Defer {
        F f;
        ~Defer() noexcept { f(); }

        Defer(const Defer&) = delete;
        Defer& operator=(const Defer&) = delete;
        Defer(Defer&&) = delete;
        Defer& operator=(Defer&&) = delete;
    };

    // when seeing Defer(some_lambda) convet to Defer<some_lambda>
    // -> overloaded to mean deduction guard arrow (bruh this is new to me)
    template <class F>
    Defer(F) -> Defer<F>;
}