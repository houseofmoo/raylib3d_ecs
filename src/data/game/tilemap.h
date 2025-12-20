#pragma once
#include <cmath>
#include <cinttypes>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "raylib.h"

    // TEMP MASKS
    constexpr uint8_t OPEN = 0;
    constexpr uint8_t TERRAIN = 1 << 0;
    constexpr uint8_t HOLE =    1 << 1;
    constexpr uint8_t LAVA =    1 << 2;

    struct Tilemap {
        int width;
        int height;
        Vector3 grid_max;
        Vector3 grid_min;
        float tile_size;
        std::vector<uint8_t> mask; // [tz * width + tx]

        Tilemap(Vector3 grid_min, Vector3 grid_max, float tile_size = 1.0f) : 
            width((int)std::ceil((grid_max.x - grid_min.x) / tile_size)), 
            height((int)std::ceil((grid_max.z - grid_min.z) / tile_size)), 
            grid_min(grid_min),
            grid_max(grid_max),
            tile_size(tile_size),
            mask(width * height, OPEN) {
                // width = (int)std::ceil((grid_max.x - grid_min.x) / tile_size); 
                // height = (int)std::ceil((grid_max.z - grid_min.z) / tile_size);
                // mask.assign((size_t)width * (size_t)height, OPEN);
                // std::cout << "width: " << width << "\n";
                // std::cout << "height: " << height << "\n";
            }

        // conversions
        inline int WorldToTileX(float world_x) const {
            return (int)std::floor((world_x - grid_min.x) / tile_size);
        }

        inline int WorldToTileZ(float world_z) const {
            return (int)std::floor((world_z - grid_min.z) / tile_size);
        }

        // bounds checks
        inline bool InBoundsWorld(float world_x, float world_z) const {
            return (world_x >= grid_min.x && world_x < grid_max.x &&
                    world_z >= grid_min.z && world_z < grid_max.z);
        }

        inline bool InBoundsTile(int tile_x, int tile_z) const {
            return !(
                tile_x < 0 ||
                tile_z < 0 ||
                tile_x >= width ||
                tile_z >= height
            );
        }

        // tile space access
        inline uint8_t GetMaskTile(int tile_x, int tile_z) const {
            if (!InBoundsTile(tile_x, tile_z)) return TERRAIN;
            return mask[(size_t)tile_z * (size_t)width + (size_t)tile_x];
        }

        inline bool IsBlockedTile(int tile_x, int tile_z) const {
            return (GetMaskTile(tile_x, tile_z) & TERRAIN) != 0;
        }

        inline void SetMaskTile(int tile_x, int tile_z, uint8_t bits) {
            if (!InBoundsTile(tile_x, tile_z)) {
                std::cout << "bad coords\n";
                return;
            }
            mask[(size_t)tile_z * (size_t)width + (size_t)tile_x] |= bits;
        }

        // world space access
        inline uint8_t GetMaskWorld(float world_x, float world_z) const {
            int tx = WorldToTileX(world_x);
            int tz = WorldToTileZ(world_z);
            return GetMaskTile(tx, tz);
        }

        inline bool IsBlockedWorld(float world_x, float world_z) const {
            uint8_t blocked = GetMaskWorld(world_x, world_z);
            return (GetMaskWorld(world_x, world_z) & TERRAIN) != 0;
        }

        inline void SetMaskWorld(float world_x, float world_z, uint8_t bits) {
            int tx = WorldToTileX(world_x);
            int tz = WorldToTileZ(world_z);
            SetMaskTile(tx, tz, bits);
        }

        inline void SetAreaMask(Vector3 world_pos, Vector3 size, uint8_t bits = TERRAIN) {
            // world AABB in XZ
            float minX = world_pos.x - (size.x * 0.5f);
            float maxX = world_pos.x + (size.x * 0.5f);
            float minZ = world_pos.z - (size.z * 0.5f);
            float maxZ = world_pos.z + (size.z * 0.5f);

            // convert to tile coords
            int start_x = WorldToTileX(minX);
            int end_x   = WorldToTileX(maxX);
            int start_z = WorldToTileZ(minZ);
            int end_z   = WorldToTileZ(maxZ);

            // clamp to map
            start_x = std::max(0, start_x);
            start_z = std::max(0, start_z);
            end_x   = std::min(width - 1, end_x);
            end_z   = std::min(height - 1, end_z);

            std::cout << "maxX: " << maxX << "\n";
            std::cout << "minX: " << minX << "\n";
            std::cout << "maxZ: " << maxZ << "\n";
            std::cout << "minZ: " << minZ << "\n";
            
            std::cout << "start_x: " << start_x << "\n";
            std::cout << "end_x: " << end_x << "\n";
            std::cout << "start_z: " << start_z << "\n";
            std::cout << "end_z: " << end_z << "\n";
            std::cout << "\n";

            for (int z = start_z; z <= end_z; ++z) {
                for (int x = start_x; x <= end_x; ++x) {
                    std::cout << "setting index [" << x << "," << z << "]\n";
                    SetMaskTile(x, z, bits);
                }
            }
        }
        
        inline void ResetMask() {
            std::fill(mask.begin(), mask.end(), OPEN);
        }

        inline void DrawTileMap(const char* path = "tilemap.txt") const {
            // std::ofstream file(path);
            // for (int tz = 0; tz < height; ++tz) {
            //     for (int tx = 0; tx < width; ++tx) {
            //         size_t index = (size_t)tz * (size_t)width + (size_t)tx;
            //         file << "mask[" << index << "] = " << (int)mask[index] << "\n";
            //     }
            //     file << '\n';
            // }
            // file.close();
            std::ofstream file(path);
            for (int tz = 0; tz < height; ++tz) {
                for (int tx = 0; tx < width; ++tx) {
                    file << (IsBlockedTile(tx, tz) ? '#' : '_');
                }
                file << '\n';
            }
            file.close();
        }
    };