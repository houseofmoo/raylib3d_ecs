#include "draw/hud/hud.h"

#include "raylib.h"
#include "draw/rgui.h"
#include "data/global_data.h"
#include "components/components.h"

namespace draw {
    void DrawHUD(strg::Registry& world, const float delta_time) {
        auto& pstats = world.GetComponent<cmpt::Stats>(gd::player.id);
        auto& pinfo = world.GetComponent<cmpt::Player>(gd::player.id);

        rgui::Label(rgui::Fmt("FPS: %d", GetFPS()),                       Vector2{  20, 20});
        rgui::Label(rgui::Fmt("Entities: %d", gd::game.entity_count), Vector2{ 120, 20});

        rgui::Label(rgui::Fmt("Difficulty: %d", gd::game.difficulty),      Vector2{ 20, 60});
        rgui::Label(rgui::Fmt("Enemies defeated: %d", pinfo.enemies_defeated), Vector2{ 20, 80});

        rgui::Label(rgui::Fmt("Level: %d", pinfo.level),                      Vector2{ 20, 120});
        rgui::Label(rgui::Fmt("HP: %d/%d", pstats.current_hp, pstats.max_hp), Vector2{ 20, 140});

        rgui::Label(rgui::Fmt("%.2f +Damage", pstats.damage_modifier),               Vector2{ 20, 180});
        rgui::Label(rgui::Fmt("%.2f +Attack Speed", pstats.attack_speed_modifier),   Vector2{ 20, 200});
        rgui::Label(rgui::Fmt("%.2f +Move Speed", pstats.move_speed_modifier),       Vector2{ 20, 220});
        rgui::Label(rgui::Fmt("%.2f +Dash Speed", pstats.dash_speed_modifier),       Vector2{ 20, 240});
        rgui::Label(rgui::Fmt("%.2f +Pickup Range", pstats.pickup_range_modifier),   Vector2{ 20, 260});

        // draw notifications
        int y_offset = gd::screen_size.y - 50;  // start from bottom of the screen 
        int x_pos = 50;                            // left side
        for (auto noti : world.View<cmpt::Notification, 
                                    cmpt::Lifetime>()) {
            auto& lifetime = world.GetComponent<cmpt::Lifetime>(noti);
            lifetime.countdown -= delta_time;
            if (lifetime.countdown <= 0.0) {
                world.AddComponent<tag::Destroy>(noti);
                continue;
            }
            
            auto& notification = world.GetComponent<cmpt::Notification>(noti);
            rgui::Label(
                notification.msg.data(), 
                Vector2{ static_cast<float>(x_pos), static_cast<float>(y_offset) }
            );
            y_offset -= 20;
        }
    }
}
