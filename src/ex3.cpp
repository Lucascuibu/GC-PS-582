#include "ex3.h"
#include "geometrycentral/surface/halfedge_element_types.h"
#include <math.h>
#include <random>

void CurveSmoothing::generate_curve(const int _num_vertices) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0., 5 * 3e-2);

  Point center = {0., 0., 0.};
  double radius = 0.3;

  //   std::vector<Vertex> vhs;
  //   Point pt;
  //   for (int i = 0; i < _num_vertices; ++i) {
  //     double frac = static_cast<double>(i) /
  //     static_cast<double>(_num_vertices); pt[0] =
  //         center[0] + radius * cos(2. * M_PI * frac) +
  //         distribution(generator);
  //     pt[1] =
  //         center[0] + radius * sin(2. * M_PI * frac) +
  //         distribution(generator);

  //     vhs.push_back(mesh_.add_vertex(pt));
  //   }

  for (int i = 0; i < _num_vertices; ++i) {
    auto a = geometry->vertexPositions[i];
    auto b = geometry->vertexPositions[(i + 1) % _num_vertices];
    points.push_back(a);
    points.push_back(b);
    VertexData<Vector3> randompointsPositions(trimesh);
    // size_t idx = 0;
    // for (Vertex v : trimesh.vertices()) {
    //     const glm::vec3& pos = geometry->vertexPositions.getValue(idx);
    //     randompointsPositions[v] = Vector3{pos.x, pos.y, pos.z};
    //     idx++;
    // }
    geometry = std::make_unique<VertexPositionGeometry>(trimesh,
                                                        randompointsPositions);
  }
}

// ============================================================================
// Exercise 4 : fill the 2 functions below (see PDF for instructions)
// Store the smoothed curve points in vector points_new
// Hint : try to play with epsilon
// ============================================================================

void CurveSmoothing::laplacian_smoothing(const int _num_iter,
                                         const double _epsilon) {
  unsigned int num_vertices = trimesh.nVertices();

  std::vector<Point> points_new(num_vertices, {0., 0., 0.});

  // store the old points
  std::vector<Point> points_old;
  points_old.reserve(num_vertices);
  for (auto vh : trimesh.vertices()) {
    auto p = geometry->vertexPositions[vh];
    points_old.push_back(p);
  }

  double old_length = 0.0;
  for (unsigned int i = 0; i < num_vertices; ++i) {
    old_length += util::cal_edge_length(points_old[i],
                                        points_old[(i + 1) % num_vertices]);
  }
  Point poly_center = {0., 0., 0.};
  for (auto vh : trimesh.vertices()) {
    auto p = geometry->vertexPositions[vh];
    poly_center += p;
  }
  poly_center /= num_vertices;

  for (int iter = 0; iter < _num_iter; ++iter) {

    for (unsigned int i = 0; i < num_vertices; ++i) {
      int prev = (i - 1) % num_vertices;
      int next = (i + 1) % num_vertices;
      Point avg = points_old[prev] + points_old[next];
      points_new[i] = (1 - _epsilon) * points_old[i] + (_epsilon / 2) * avg;
    }

    double new_length = 0.0;
    for (unsigned int i = 0; i < num_vertices; ++i) {
      new_length += util::cal_edge_length(points_new[i],
                                          points_new[(i + 1) % num_vertices]);
    }
    double ratio = old_length / new_length;
    std::cout << "ratio: " << ratio << std::endl;

    Point new_center{0., 0., 0.};
    for (unsigned int i = 0; i < num_vertices; ++i) {
      points_new[i] *= ratio;
      new_center += points_new[i];
    }
    new_center /= num_vertices;

    for (unsigned int i = 0; i < num_vertices; ++i) {
      points_new[i] += poly_center - new_center;
    }

    // points_old = points_new;
  }

  if (!points_new.empty()) {
    VertexData<Vector3> newPositions(trimesh);
    int j = 0;
    for (Vertex v : trimesh.vertices()) {
      newPositions[v] =
          Vector3{points_new[j][0], points_new[j][1], points_new[j][2]};
      j++;
    }
    geometry = std::make_unique<VertexPositionGeometry>(trimesh, newPositions);
  }
}

void CurveSmoothing::osculating_circle(const int _num_iter,
                                       const double _epsilon) {
  unsigned int num_vertices = trimesh.nVertices();

  std::vector<Point> points_new(num_vertices, {0., 0., 0.});

  std::vector<Point> points_old;
  points_old.reserve(num_vertices);
  for (auto vh : trimesh.vertices()) {
    auto p = geometry->vertexPositions[vh];
    points_old.push_back(p);
  }
  // points_old.push_back(mesh_.vertex(vh));

  Point poly_center{0., 0., 0.};
  for (auto vh : trimesh.vertices()) {
    auto p = geometry->vertexPositions[vh];
    poly_center += p;
    // poly_center += mesh_.vertex(vh);
  }
  poly_center /= num_vertices;

  double old_length = 0.0;
  for (unsigned int i = 0; i < num_vertices; ++i) {
    old_length += util::cal_edge_length(points_old[i],
                                        points_old[(i + 1) % num_vertices]);
  }

  for (int iter = 0; iter < _num_iter; ++iter) {
    for (unsigned int i = 0; i < num_vertices; ++i) {
      int prev = (i - 1) % num_vertices;
      int next = (i + 1) % num_vertices;
      Point center = util::circumscribedCircle(points_old[prev], points_old[i],
                                               points_old[next]);
      Point dir = center - points_old[i];
      double deno =
          std::pow(dir[0], 2) + std::pow(dir[1], 2) + std::pow(dir[2], 2);

      points_new[i] = points_old[i] + _epsilon * dir / deno;
    }

    double new_length = 0.0;
    for (unsigned int i = 0; i <= num_vertices; ++i) {
      new_length += util::cal_edge_length(points_new[i],
                                          points_new[(i + 1) % num_vertices]);
    }
    double ratio = old_length / new_length;

    Point new_center = {0., 0., 0.};
    for (unsigned int i = 0; i < num_vertices; ++i) {
      points_new[i] *= ratio;
      new_center += points_new[i];
    }
    new_center /= num_vertices;

    for (unsigned int i = 0; i < num_vertices; ++i) {
      points_new[i] += (poly_center - new_center);
    }

    // points_old = points_new;
  }

  if (!points_new.empty()) {
    VertexData<Vector3> newPositions(trimesh);
    int j = 0;
    for (Vertex v : trimesh.vertices()) {
      newPositions[v] =
          Vector3{points_new[j][0], points_new[j][1], points_new[j][2]};
      j++;
    }
    geometry = std::make_unique<VertexPositionGeometry>(trimesh, newPositions);
  }
}