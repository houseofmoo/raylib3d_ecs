#include "draw/entities/draw_entities.h"

#include <utility>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "components/components.h"
#include "data/global_data.h"
#include "utils/debug.h"

namespace draw {
    Color GetDrawColor(strg::Registry& world, const float delta_time, const Entity id, const Color color) {
        Color new_color = color;
        if (world.HasComponent<cmpt::DamageFlash>(id)) {
            auto& flash = world.GetComponent<cmpt::DamageFlash>(id);
            flash.duration -= delta_time;
            if (flash.duration < 0.0f) {
                world.RemoveComponent<cmpt::DamageFlash>(id);
            }
            new_color = WHITE;
        }

        if (world.HasComponent<cmpt::Invulnerable>(id)) {
            new_color.a = 128;
        }

        return new_color;
    }

    std::pair<Vector3, float> GetRotation(Quaternion rotation) {
        Vector3 axis;
        float angle = 0.0f;
        QuaternionToAxisAngle(rotation, &axis, &angle);
        axis = Vector3Normalize(axis);
        return { axis, angle };
    }

    void DrawStaticWiredModel(const cmpt::Transform& trans, const cmpt::Draw& draw) {
        DrawCubeV(trans.position, draw.size, draw.color);
        DrawCubeWiresV(trans.position, draw.size, BLACK);
    }

    void DrawWiredModelEx(const cmpt::Transform& trans, const cmpt::Draw& draw, const Color color) {
        // get model rotation
        auto [axis, angle] = GetRotation(trans.rotation);

        // draw model
        DrawModelEx(*draw.model, trans.position, axis, RAD2DEG * angle, draw.scale, color);

        // draw wire free that rotates with entity
        rlPushMatrix();
            rlTranslatef(trans.position.x, trans.position.y, trans.position.z);   // pivot at model position
            rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);                   // apply same rotation
            DrawCubeWiresV({0,0,0}, draw.size, BLACK);                            // draw in local space
        rlPopMatrix();
    }

    void DrawPlayAreaGround() {
        float width = data::cnst::PLAY_AREA.max.x - data::cnst::PLAY_AREA.min.x;
        float length = data::cnst::PLAY_AREA.max.z - data::cnst::PLAY_AREA.min.z; 
        DrawCube(Vector3{0.0f, -0.51f, 0.0f}, width, 1.0f, length, GRAY);

        // grid lines in play area
        for (int x = (int)data::cnst::PLAY_AREA.min.x; x < data::cnst::PLAY_AREA.max.x; x+=2) {
            Vector3 top =    { (float)x, 0.0f, data::cnst::PLAY_AREA.min.z };
            Vector3 bottom = { (float)x, 0.0f, data::cnst::PLAY_AREA.max.z };
            DrawLine3D(top, bottom, BLACK);
        }
        for (int z = (int)data::cnst::PLAY_AREA.min.z; z < data::cnst::PLAY_AREA.max.z; z+=2) {
            Vector3 top =    { data::cnst::PLAY_AREA.min.x, 0.0f, (float)z };
            Vector3 bottom = { data::cnst::PLAY_AREA.max.x, 0.0f, (float)z };
            DrawLine3D(top, bottom, BLACK);
        }
    }

    void DrawPlayAreaWalls() {
        Vector3 position = { data::cnst::PLAY_AREA.min.x - 0.5f, 3.0f, 0.0f };
        Vector3 size = { 1.0f, 6.0f, data::cnst::PLAY_AREA.max.z - data::cnst::PLAY_AREA.min.z };
        DrawCubeV(position, size, DARKGRAY);
        DrawCubeWiresV(position, size, BLACK);

        position = { data::cnst::PLAY_AREA.max.x + 0.5f, 3.0f, 0.0f };
        size = { 1.0f, 6.0f, data::cnst::PLAY_AREA.max.z - data::cnst::PLAY_AREA.min.z };
        DrawCubeV(position, size, DARKGRAY);
        DrawCubeWiresV(position, size, BLACK);

        position = {  0.0f, 3.0f, data::cnst::PLAY_AREA.min.z - 0.5f };
        size = { data::cnst::PLAY_AREA.max.x - data::cnst::PLAY_AREA.min.x, 6.0f, 1.0f };
        DrawCubeV(position, size, DARKGRAY);
        DrawCubeWiresV(position, size, BLACK);

        DrawLine3D(
            Vector3{ data::cnst::PLAY_AREA.min.x + 0.01f, 6.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            Vector3{ data::cnst::PLAY_AREA.min.x + 0.01f, 0.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            BLACK
        );
        DrawLine3D(
            Vector3{ data::cnst::PLAY_AREA.max.x - 0.01f, 6.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            Vector3{ data::cnst::PLAY_AREA.max.x - 0.01f, 0.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            BLACK
        );
    }

    void DrawEntities(strg::Registry& world, const float delta_time) {
            PROFILE_SCOPE("DrawEntities()");
            gd::game.entity_count = 0;
            gd::game.enemy_count = 0;

            for (auto entity : world.View<cmpt::Draw, cmpt::Transform>()) {
                auto& draw = world.GetComponent<cmpt::Draw>(entity);
                auto& trans = world.GetComponent<cmpt::Transform>(entity);

                gd::game.entity_count += 1;
                if (draw.model == nullptr) { // only terrain does not have a draw model atm
                    // terrain does not have a model atm
                    DrawStaticWiredModel(trans, draw);
                } else {
                    if (world.HasComponent<tag::Enemy>(entity)) gd::game.enemy_count += 1;
                    DrawWiredModelEx(trans, draw, GetDrawColor(world, delta_time, entity, draw.color));
                }
            }
            
            DrawPlayAreaGround();
            DrawPlayAreaWalls();
    }
}