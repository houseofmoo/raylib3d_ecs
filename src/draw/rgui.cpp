#include "draw/rgui.h"
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"
#include "boxonbox.h"


namespace rgui {
    ::GuiTextAlignment CopnvertAlignment(const HAlign alignment) {
        switch (alignment) {
            case HAlign::Left: return TEXT_ALIGN_LEFT;
            case HAlign::Center: return TEXT_ALIGN_CENTER;
            case HAlign::Right: return TEXT_ALIGN_RIGHT;
        }
        return TEXT_ALIGN_LEFT;
    }

    float ConvertPositionX(const HAlign alignment, const float pos_x, const float width) {
         switch (alignment) {
            case HAlign::Left: return pos_x;
            case HAlign::Center: return pos_x - (width * 0.5f);
            case HAlign::Right: return pos_x + width;
        }
        return pos_x;
    }

    void LoadStyles() {
        ::GuiLoadStyleBoxOnBox();
    }

    void LockGui() {
        ::GuiLock();
    }

    void UnlockGui() {
        ::GuiUnlock();
    }

    int MeasureTextWidth(const char* text) {
        Font font = ::GuiGetFont();
        return ::MeasureText(text, font.baseSize);
    }

    void Label(const char* text, Vector2 pos, HAlign align, Color tint) {
        int width = MeasureTextWidth(text);
        ::GuiDrawText(
            text, 
            Rectangle{ 
                .x = ConvertPositionX(align, pos.x, width), 
                .y = pos.y, 
                .width = (float)width, 
                .height = 25.0f
            }, 
            CopnvertAlignment(align),
            tint
        );
    }

    void Label(std::string_view text, Vector2 pos, HAlign align, Color tint) {
        Label(text.data(), pos, align, tint);
    }

    void Label(const char* text, Rectangle rec, HAlign align, Color tint) {
        ::GuiDrawText(
            text, 
            Rectangle{ 
                .x = ConvertPositionX(align, rec.x, rec.width), 
                .y = rec.y, 
                .width = rec.width, 
                .height = rec.height
            }, 
            CopnvertAlignment(align),
            tint
        );
    }

    void Label(std::string_view text, Rectangle rec, HAlign align, Color tint) {
        Label(text.data(), rec, align, tint);
    }

    bool Button(const char* text, Vector2 pos, HAlign align) {
        int width = MeasureTextWidth(text);
        return ::GuiButton(
            Rectangle{
                .x = ConvertPositionX(align, pos.x, width),
                .y = pos.y,
                .width = static_cast<float>(width),
                .height = 25.0f
            }, 
            text
        );
    }

    bool Button(std::string_view text, Vector2 pos, HAlign align) {
        return Button(text.data(), pos, align);
    }

    bool Button(const char* text, Rectangle rec, HAlign align) {
        rec.x = ConvertPositionX(align, rec.x, rec.width);
        return ::GuiButton(rec, text);
    }

    bool Button(std::string_view text, Rectangle rec, HAlign align) {
        return Button(text.data(), rec, align);
    }

    //void Panel(Rectangle rec) {
    //     return ::GuiPanel(rec);    
    // }

    //void SeperatorH(float x, float y, float w, float thickness, Color c) {
    //     return ::DrawRectangleV(Vector2{x. y}, vector2 {w, thickness }, c);
    // }

    std::string_view Fmt(const char* fmt, ...) {
        std::va_list args;
        va_start(args, fmt);
        auto sv = VFmt(fmt, args);
        va_end(args);
        return sv;
    }

    std::string_view VFmt(const char* fmt, std::va_list args) {
        thread_local char buf[512]; // increase for larger line buffer

        std::va_list copy;
        va_copy(copy, args);
        int n = std::vsnprintf(buf, sizeof(buf), fmt, copy);
        va_end(copy);

        if (n < 0) return {};
        if (n >= (int)sizeof(buf)) n = (int)sizeof(buf) - 1;
        return std::string_view(buf, (size_t)n);
    }
}
