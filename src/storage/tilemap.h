#pragma once
#include <cmath>
#include <cinttypes>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "raylib.h"
#include "utils/debug.h"

namespace strg {
    // TEMP MASKS
    //using TileType = uint8_t;
    constexpr uint8_t OPEN    = 0;
    constexpr uint8_t OOB     = 1 << 0; // out of bounds
    constexpr uint8_t LOW     = 1 << 1; // bullets can travel over this
    constexpr uint8_t TERRAIN = 1 << 2; // arch movers can move over this
    constexpr uint8_t HOLE    = 1 << 3;
    constexpr uint8_t LAVA    = 1 << 5;

    // tile types would be OOB, OPEN, TERRAIN, HOLE, DAMAGE?

    enum class TileType : uint8_t {
        OPEN,
        OOB,
        TERRAIN,
        HOLE,
        DAMAGE
    };

    struct Tile {
        TileType type;
        float height;

        bool Is(TileType other_type) {
            return type == other_type;
        }
    };

    class Tilemap {
        int width;
        int height;
        Vector3 grid_max;
        Vector3 grid_min;
        float tile_size;
        std::vector<Tile> tiles; // [tz * width + tx]

        public:
        Tilemap(Vector3 grid_min, Vector3 grid_max, float tile_size = 1.0f);
        int WorldToTileX(const float world_x) const;
        int WorldToTileZ(const float world_z) const;
        bool InBoundsWorld(const float world_x, const float world_z) const;
        bool InBoundsTile(const int tile_x, const int tile_z) const;
        Tile GetTile(const float world_x, const float world_z) const;
        Tile GetTile(const int tile_x, const int tile_z) const;
        void SetTile(const int tile_x, const int tile_z, const Tile tile);
        bool ValidMove(const float world_x, const float world_z, const float height) const;
        bool ValidMove(const Vector3 position, const float height) const;
        bool ValidMove(const Vector3 posistion, const Vector3 collider) const;
        void SetAreaTiles(const Vector3 world_pos, const Vector3 size, const TileType type = TileType::TERRAIN);
        void ResetTiles();
        void DrawTileMap(const char* path = "tilemap.txt") const;
    };
}