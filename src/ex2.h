#ifndef OPENFLIPPER_ISOCONTOURING_HH
#define OPENFLIPPER_ISOCONTOURING_HH

#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/vertex_position_geometry.h"
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/meshio.h>
#include <glm/glm.hpp>
#include <tuple>
#include <polyscope/render/color_maps.h>

#include "geometrycentral/utilities/vector3.h"
#include "polyscope/render/managed_buffer.h"
#include "polyscope/curve_network.h"

#include "polyscope/surface_mesh.h"
#include "utils.h"

using namespace geometrycentral;
using namespace geometrycentral::surface;

class IsoContouring {
public:
  using Point = geometrycentral::Vector3;

  IsoContouring(SurfaceMesh &_trimesh)
      : trimesh(_trimesh) {
    vertexColors = std::vector<Point>(trimesh.nVertices());
  }
  ~IsoContouring() {}

  void show_isovalue_and_level_set();

  int function_id_ = 0;

  auto get_color() { return vertexColors; }

  void init_geometry(polyscope::render::ManagedBuffer<glm::vec3> vertexPositions) {
    VertexData<Vector3> gcPositions(trimesh);
    size_t idx = 0;
    for (Vertex v : trimesh.vertices()) {
        const glm::vec3& pos = vertexPositions.getValue(idx);
        gcPositions[v] = Vector3{pos.x, pos.y, pos.z};
        idx++;
    }
    geometry = std::make_unique<VertexPositionGeometry>(trimesh, gcPositions);
  }

  std::vector<std::array<double, 3>> vertcies;
std::vector<std::array<size_t, 2>> edges;

private:
  double iso_value(const Point &_pt) const;

  void show_isovalue();

  void compute_segment_points();

  void create_level_set0_segments();

 

private:
  std::unique_ptr<VertexPositionGeometry> geometry;
  SurfaceMesh &trimesh;
  std::vector<Point> vertexColors;

  std::vector<Point> segment_points_;
};

#endif // OPENFLIPPER_ISOCONTOURING_HH
