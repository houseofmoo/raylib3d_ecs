#include <array>
#include <cstdint>
#include <cmath>
#include "data/entity.h"
#include "utils/debug.h"

static constexpr int kInvalid = -1;

namespace strg {
    struct Grid2D {
        // Tune these:
        static constexpr int kMaxEntities = 5000;
        static constexpr int kWorldWUnits = 120;
        static constexpr int kWorldHUnits = 60;
        static constexpr int kCellSizeUnits = 2;

        static constexpr int kCols = (kWorldWUnits + kCellSizeUnits - 1) / kCellSizeUnits;
        static constexpr int kRows = (kWorldHUnits + kCellSizeUnits - 1) / kCellSizeUnits;
        static constexpr int kMaxCells = kCols * kRows;

        float originX = -kWorldWUnits * 0.5f;
        float originZ = -kWorldHUnits * 0.5f;

        std::array<int, kMaxCells> head;
        std::array<int, kMaxEntities> next;

        void clear() {
            head.fill(kInvalid);
            next.fill(kInvalid);
        }

        static int clampi(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }
        static int cell_index(int cx, int cz) { return cz * kCols + cx; }

        int pos_to_cell(float x, float z) const {
            int cx = int((x - originX) / kCellSizeUnits);
            int cz = int((z - originZ) / kCellSizeUnits);
            cx = clampi(cx, 0, kCols - 1);
            cz = clampi(cz, 0, kRows - 1);
            return cell_index(cx, cz);
        }

        void insert(Entity e, float x, float z) {
            if ((int)e < 0 || (int)e >= kMaxEntities) {
                PRINT("insert to Grid2D failed");
                return;
            }
            int c = pos_to_cell(x, z);
            next[e] = head[c];
            head[c] = (int)e;
        }

        template <class Fn>
        void for_each_candidate_pair(Fn&& fn) const {
            // Check only "forward" neighbors to avoid duplicate pairs
            constexpr int neigh[4][2] = {
                {1, 0},   // right
                {0, 1},   // up
                {1, 1},   // up-right
                {1,-1},   // down-right
            };

            for (int cz = 0; cz < kRows; ++cz) {
                for (int cx = 0; cx < kCols; ++cx) {
                    int c = cell_index(cx, cz);

                    // pairs within cell
                    for (int a = head[c]; a != kInvalid; a = next[a]) {
                        for (int b = next[a]; b != kInvalid; b = next[b]) {
                            fn((Entity)a, (Entity)b);
                        }
                    }

                    // pairs with neighbor cells
                    for (auto& off : neigh) {
                        int nx = cx + off[0];
                        int nz = cz + off[1];
                        if (nx < 0 || nx >= kCols || nz < 0 || nz >= kRows) continue;
                        int nc = cell_index(nx, nz);

                        for (int a = head[c]; a != kInvalid; a = next[a]) {
                            for (int b = head[nc]; b != kInvalid; b = next[b]) {
                                fn((Entity)a, (Entity)b);
                            }
                        }
                    }
                }
            }
        }
    };
}