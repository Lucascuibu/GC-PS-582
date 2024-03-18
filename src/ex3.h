#ifndef OPENFLIPPER_CURVESMOOTHING_HH
#define OPENFLIPPER_CURVESMOOTHING_HH

// #include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>

#include "geometrycentral/utilities/vector3.h"
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

class CurveSmoothing {
public:
   using Point = geometrycentral::Vector3;

    CurveSmoothing(SurfaceMesh &_trimesh): trimesh(_trimesh) {}
    ~CurveSmoothing(){}

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
public:
    void generate_curve(const int _num_vertices);

    void laplacian_smoothing(const int _num_iter, const double _epsilon);

    void osculating_circle(const int _num_iter, const double _epsilon);


private:
std::unique_ptr<VertexPositionGeometry> geometry;
  SurfaceMesh &trimesh;
  std::vector<Point> vertexColors;
    std::vector<Point> points;
};
#endif //OPENFLIPPER_CURVESMOOTHING_HH
