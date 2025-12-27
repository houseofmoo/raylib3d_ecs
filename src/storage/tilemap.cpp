#include "storage/tilemap.h"
#include <iomanip>
#include "utils/rl_utils.h"
#include "utils/debug.h"


namespace strg {
    Tilemap::Tilemap(Vector3 grid_min, Vector3 grid_max, float tile_size) : 
            width((int)std::ceil((grid_max.x - grid_min.x) / tile_size)), 
            height((int)std::ceil((grid_max.z - grid_min.z) / tile_size)), 
            grid_min(grid_min),
            grid_max(grid_max),
            tile_size(tile_size),
            tiles(width * height, Tile{.type = TileType::OPEN, .height = 0.0f }) {}

    int Tilemap::WorldToTileX(const float world_x) const {
        return (int)std::floor((world_x - grid_min.x) / tile_size);
    }

    int Tilemap::WorldToTileZ(const float world_z) const {
        return (int)std::floor((world_z - grid_min.z) / tile_size);
    }

    bool Tilemap::InBoundsWorld(const float world_x, const float world_z) const {
        return (world_x >= grid_min.x && world_x < grid_max.x &&
                world_z >= grid_min.z && world_z < grid_max.z);
    }

    bool Tilemap::InBoundsTile(const int tile_x, const int tile_z) const {
        return !(
            tile_x < 0 ||
            tile_z < 0 ||
            tile_x >= width ||
            tile_z >= height
        );
    }

    Tile Tilemap::GetTile(const int tile_x, const int tile_z) const {
        if (!InBoundsTile(tile_x, tile_z)) {
            return Tile{.type = TileType::OOB, .height = 0.0f };
        }
        return tiles[(tile_z * width) + tile_x];
    }

    Tile Tilemap::GetTile(const float world_x, const float world_z) const {
        int tile_x = WorldToTileX(world_x);
        int tile_z = WorldToTileZ(world_z);
        return GetTile(tile_x, tile_z);
    }

    void Tilemap::SetTile(const int tile_x, const int tile_z, const Tile tile) {
        if (!InBoundsTile(tile_x, tile_z)) {
            PRINT("Attempted to set OOB tile in SetTile()");
            return;
        }
        tiles[(tile_z * width) + tile_x] = tile;
    }

    bool Tilemap::ValidMove(const float world_x, const float world_z, const float height) const {
        // height = trans.position.y - (col.size.y * 0.5f)
        if (!InBoundsWorld(world_x, world_z)) return false;

        Tile tile = GetTile(world_x, world_z);
        switch (tile.type) {
            case TileType::OPEN: return true;
            case TileType::OOB: return false;
            case TileType::TERRAIN: return tile.height < height;
            case TileType::HOLE: return true; // will die from fall?
            case TileType::DAMAGE: return true; // will get damaged somehow?
            default: return true; // catch all: allow movement here
        }
    }
    
    bool Tilemap::ValidMove(const Vector3 position, const float height) const {
        // height = trans.position.y - (col.size.y * 0.5f)
        return ValidMove(position.x, position.z, height);
    }

    bool Tilemap::ValidMove(const Vector3 position, const Vector3 collider) const {
        // height = trans.position.y - (col.size.y * 0.5f)
        return ValidMove(position.x, position.z, utils::GetEntityHeight(position, collider));
    }

    void Tilemap::SetAreaTiles(const Vector3 world_pos, const Vector3 size, const TileType type) {
        float min_x = world_pos.x - (size.x * 0.5f);
        float max_x = world_pos.x + (size.x * 0.5f);
        float min_z = world_pos.z - (size.z * 0.5f);
        float max_z = world_pos.z + (size.z * 0.5f);
        float height = world_pos.y + (size.y * 0.5);

        // to tile coords
        int start_x = WorldToTileX(min_x);
        int end_x   = WorldToTileX(max_x - 0.001f);
        int start_z = WorldToTileZ(min_z);
        int end_z   = WorldToTileZ(max_z - 0.001f);

        for (int z = start_z; z <= end_z; ++z) {
            for (int x = start_x; x <= end_x; ++x) {
                SetTile(x, z, Tile{ .type = type, .height = height });
            }
        }
    }

    void Tilemap::ResetTiles() {
        std::fill(tiles.begin(), tiles.end(), Tile{.type = TileType::OPEN, .height = 0.0f });
    }

    void Tilemap::DrawTileMap(const char* path) const {
        std::ofstream file(path);
        for (int tz = 0; tz < height; ++tz) {
            for (int tx = 0; tx < width; ++tx) {
                auto tile = GetTile(tx, tz);
                if (tile.height <= 0.0f) {
                    file << "_";
                } else {
                    //file << std::fixed << std::setprecision(1);
                    file << (int)tile.height;
                }
            }
            file << '\n';
        }
        file.close();
    }
}
