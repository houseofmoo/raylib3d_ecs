#include "state/menus/weapon_select/weapon_select.h"

#include <memory>
#include "raylib.h"
#include "draw/rgui.h"
#include "data/global_data.h"
#include "spawners/equip/weapon/weapons.h"
#include "utils/debug.h"

namespace state {
    DISABLE_UNUSED_WARNINGS

    void WeaponSelectMenu::HandleInput(StateContext& ctx) {}

    void WeaponSelectMenu::Update(StateContext& ctx) {}

    void WeaponSelectMenu::Draw(StateContext& ctx) {
        // darken background
        DrawRectangle(0, 0,  gd::screen_size.x, gd::screen_size.y, Color{0,0,0,128});

        // display choices
        auto rec = Rectangle{ (gd::screen_size.x * 0.5f) - 75.0f, 300.0f, 200.0f, 40.0f };
        if (rgui::Button("Pistol", rec, rgui::HAlign::Center)) {
            spwn::weapon::EquipPistol(
                ctx.world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            ctx.cmd.Pop();
        }

        rec.y += 50.0f;
        if (rgui::Button("Shotgun", rec, rgui::HAlign::Center)) {
            spwn::weapon::EquipShotgun(
                ctx.world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            ctx.cmd.Pop();
        }

        rec.y += 50.0f;
        if (rgui::Button("Rifle", rec, rgui::HAlign::Center)) {
            spwn::weapon::EquipRifle(
                ctx.world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            ctx.cmd.Pop();
        }

        rec.y += 50.0f;
        if (rgui::Button("SMG", rec, rgui::HAlign::Center)) {
            spwn::weapon::EquipSMG(
                ctx.world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            ctx.cmd.Pop();
        }

        rec.y += 50.0f;
        if (rgui::Button("Grenade Launcher", rec, rgui::HAlign::Center)) {
            spwn::weapon::EquipGrenadeLauncher(
                ctx.world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            ctx.cmd.Pop();
        }
    }
    RESTORE_WARNINGS
}
