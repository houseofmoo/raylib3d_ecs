// #pragma once
// #include <cinttypes>
// #include <array>
// #include "raylib.h"
// using Entity = uint32_t;
// // we only care about 2D positions in a 3D area, ignoring height

// struct Point {
//     Entity id;                  // which entity does this point represent
//     Vector2 position;           // where this entity is in space (x, 0.0f, z) - ignore height (y)
//     BoundingBox collider_box;   // box that represends this entities collider
// };

// struct Rectangle {
//     Vector2 position;
//     Vector2 size;
// };

// constexpr int MAX_CAPACITY = 4;
// struct QuadTree {
//     Rectangle boundary;
//     std::array<Point, MAX_CAPACITY> points;
//     uint32_t num_points;

//     QuadTree* northwest;
//     QuadTree* northwest;
//     QuadTree* northwest;
//     QuadTree* northwest;

//     void Subdivide() {

//     }

//     void Insert(const Point& p) {
//         if (num_points < points.max_size()) {
//             points[num_points] = p;
//             ++num_points;
//             return;
//         }

//         Subdivide();
//     }
// };