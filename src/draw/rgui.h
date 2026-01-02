#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <string_view>

#include "raylib.h"

namespace rgui {
    enum class HAlign {
        Left,
        Center,
        Right,
    };

    enum class VAlign {
        Top,
        Center,
        Bottom
    };

    void LoadStyles();

    void LockGui();
    void UnlockGui();

    Rectangle AlignRect(HAlign align, float x, float y, float w, float h);
    int MeasureTextWidth(const char* text);

    void Label(const char* text, Vector2 pos, HAlign align = HAlign::Left, Color tint = WHITE);
    void Label(std::string_view text, Vector2 pos, HAlign align = HAlign::Left, Color tint = WHITE);
    void Label(const char* text, Rectangle rec, HAlign align = HAlign::Left, Color tint = WHITE);
    void Label(std::string_view text, Rectangle rec, HAlign align = HAlign::Left, Color tint = WHITE);

    bool Button(const char* text, Vector2 pos, HAlign align = HAlign::Left);
    bool Button(std::string_view text, Vector2 pos, HAlign align = HAlign::Left);
    bool Button(const char* text, Rectangle rec, HAlign align = HAlign::Left);
    bool Button(std::string_view text, Rectangle rec, HAlign align = HAlign::Left);

    //void Panel(Rectangle rec);
    //void SeperatorH(float x, float y, float w, float thickness, Color c);

    // string builders alloc free
    std::string_view Fmt(const char* fmt, ...);
    std::string_view VFmt(const char* fmt, std::va_list args);
}
