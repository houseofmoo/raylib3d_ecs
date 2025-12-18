#include "spawners/map/map.h"
#include "spawners/terrain/terrain.h"

namespace spwn::map {
     void GenerateMap(Storage::Registry& world, const BoundingBox& boundary) {
        // spawn world boundaries
        spwn::terrain::Terrain(world, Vector3{ boundary.min.x - 0.5f, 3.0f, 0.0f }, Vector3{ 1.0f, 6.0f, boundary.max.z - boundary.min.z });
        spwn::terrain::Terrain(world, Vector3{ boundary.max.x + 0.5f, 3.0f, 0.0f }, Vector3{ 1.0f, 6.0f, boundary.max.z - boundary.min.z });
        spwn::terrain::Terrain(world, Vector3{ 0.0f, 3.0f, boundary.min.z - 0.5f }, Vector3{ boundary.max.x - boundary.min.x, 6.0f, 1.0f });
        spwn::terrain::Terrain(world, Vector3{ 0.0f, 3.0f, boundary.max.z + 0.5f }, Vector3{ boundary.max.x - boundary.min.x, 6.0f, 1.0f }, false);

        // Extra terrain for funsies
        spwn::terrain::Terrain(world, Vector3{ -20.0f, 1.0f, 10.0f }, Vector3{ 1.0f, 2.0f, 15.0f });
        spwn::terrain::Terrain(world, Vector3{ 20.0f, 1.0f, 10.0f }, Vector3{ 1.0f, 2.0f, 15.0f });

        spwn::terrain::Terrain(world, Vector3{ -20.0f, 1.0f, -10.0f }, Vector3{ 1.0f, 2.0f, 15.0f });
        spwn::terrain::Terrain(world, Vector3{ 20.0f, 1.0f, -10.0f }, Vector3{ 1.0f, 2.0f, 15.0f });

        // short terrain
        spwn::terrain::Terrain(world, Vector3{ 0.0f, 0.4f, 20.0f }, Vector3{ 5.0f, 0.8f, 5.0f });
        spwn::terrain::Terrain(world, Vector3{ 0.0f, 0.4f, -20.0f }, Vector3{ 5.0f, 0.8f, 5.0f });
    }
}