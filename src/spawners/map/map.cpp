#include "spawners/map/map.h"
#include "spawners/terrain/terrain.h"
#include "data/game/game.h"

namespace spwn::map {
     void GenerateMap(Storage::Registry& world, const BoundingBox& boundary) {
        // spawn world boundaries
        data::game::terrain.ResetMask();
      
        //spwn::terrain::Terrain2(world, Vector3{ boundary.min.x - 0.5f, 3.0f, 0.0f }, Vector3{ 1.0f, 6.0f, boundary.max.z - boundary.min.z });
        //spwn::terrain::Terrain2(world, Vector3{ boundary.max.x + 0.5f, 3.0f, 0.0f }, Vector3{ 1.0f, 6.0f, boundary.max.z - boundary.min.z });
        //spwn::terrain::Terrain2(world, Vector3{ 0.0f, 3.0f, boundary.min.z - 0.5f }, Vector3{ boundary.max.x - boundary.min.x, 6.0f, 1.0f });
        //spwn::terrain::Terrain2(world, Vector3{ 0.0f, 3.0f, boundary.max.z + 0.5f }, Vector3{ boundary.max.x - boundary.min.x, 6.0f, 1.0f }, false);

        // Extra terrain for funsies
        Vector3 size = Vector3{ 1.0f, 2.0f, 15.0f };
        Vector3 pos = Vector3{ -20.0f, size.y*0.5f, 10.0f };
        spwn::terrain::Terrain2(world, pos, size);

        size = Vector3{ 1.0f, 2.0f, 15.0f };
        pos = Vector3{ 20.0f, size.y*0.5f, 10.0f };
        spwn::terrain::Terrain2(world, pos, size);

        size = Vector3{ 1.0f, 2.0f, 15.0f };
        pos = Vector3{ -20.0f, size.y*0.5f, -10.0f };
        spwn::terrain::Terrain2(world, pos, size);

        size = Vector3{ 1.0f, 2.0f, 15.0f };
        pos = Vector3{ 20.0f, size.y*0.5f, -10.0f };
        spwn::terrain::Terrain2(world, pos, size);

        // short terrain
        size = Vector3{ 5.0f, 0.5f, 5.0f };
        pos = Vector3{ 0.0f, size.y*0.5f, 20.0f };
        spwn::terrain::Terrain2(world, pos, size);

        size = Vector3{ 5.0f, 0.5f, 5.0f };
        pos = Vector3{ 0.0f, size.y*0.5f, -20.0f };
        spwn::terrain::Terrain2(world, pos, size);
    }
}