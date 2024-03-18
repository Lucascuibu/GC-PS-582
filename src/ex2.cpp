#include "ex2.h"

#include <cstddef>
#include <iostream>

void IsoContouring::show_isovalue_and_level_set() {
  show_isovalue();
  create_level_set0_segments();
}

void IsoContouring::show_isovalue() {

  std::vector<double> iso_values;
  for (auto v : trimesh.vertices()) {
    Point point = geometry->vertexPositions[v];
    auto iv = iso_value(point);
    iso_values.push_back(iv);
  }

  auto max_iv = *std::max_element(iso_values.begin(), iso_values.end());
  auto min_iv = *std::min_element(iso_values.begin(), iso_values.end());

  const double range = max_iv - min_iv;

  for (auto vh : trimesh.vertices()) {
    auto t = (iso_values[vh.getIndex()] - min_iv) / range;
    auto color = util::map_val2color(t, 0, 1.0);
    vertexColors[vh.getIndex()] = {color.r, color.g, color.b};
  }
}

double IsoContouring::iso_value(const Point &_pt) const {
  auto x = _pt[0];
  auto y = _pt[1];
  double iso = 0.;

  switch (function_id_) {
  case 0:
    iso = 0.5 * (x * x + y * y) - 1;
    break;
  case 1:
    iso = y * y - sin(x * x);
    break;
  case 2:
    iso = sin(2 * x + 2 * y) - cos(4 * x * y) + 1;
    break;
  case 3:
    iso = std::pow((3 * x * x + y * y), 2) * (y * y) -
          std::pow((x * x + y * y), 4);
    break;
  case 4:
    iso = std::pow((3 * x * x + y * y), 2) * (y * y) -
          std::pow((x * x + y * y), 4);
    break;
  }

  return iso;
}

void IsoContouring::create_level_set0_segments() {
  compute_segment_points();

  if (segment_points_.empty()) {
    std::cerr << "No segment points found" << std::endl;
    return;
  }

  if(segment_points_.size() % 2 != 0){
    std::cerr << "segment points size is not even" << std::endl;
    return;
  }

  for(size_t i = 0; i < segment_points_.size(); i+=2){
    auto p0 = segment_points_[i];
    auto p1 = segment_points_[i+1];
    vertcies.push_back({p0[0], p0[1], p0[2]});
    vertcies.push_back({p1[0], p1[1], p1[2]});
    edges.push_back({i, i+1});
  }
}

bool near_zero(double x) { return std::abs(x) < 1e-10; }

void IsoContouring::compute_segment_points() {
  segment_points_.clear();

  std::vector<Point> v_points(trimesh.nVertices());
  std::vector<std::vector<int>> triangle_ids;

  for (auto v : trimesh.vertices()) {
    v_points[v.getIndex()] = geometry->vertexPositions[v];
  }

  for (auto fh : trimesh.faces()) {
    std::vector<int> vv(3);
    int k = 0;
    for (auto heh : fh.adjacentHalfedges()) {
      vv[k] = heh.vertex().getIndex();
      k++;
    }
    triangle_ids.push_back(vv);
  }

  for (auto tri : triangle_ids) {
    double iso_a = IsoContouring::iso_value(v_points[tri[0]]);
    double iso_b = IsoContouring::iso_value(v_points[tri[1]]);
    double iso_c = IsoContouring::iso_value(v_points[tri[2]]);

    double ratio_ab = std::abs(iso_a) / std::abs(iso_a - iso_b);
    double ratio_bc = std::abs(iso_b) / std::abs(iso_b - iso_c);
    double ratio_ca = std::abs(iso_a) / std::abs(iso_a - iso_c);

    Point zero_ab =
        v_points[tri[0]] + (v_points[tri[1]] - v_points[tri[0]]) * ratio_ab;
    Point zero_bc =
        v_points[tri[1]] + (v_points[tri[2]] - v_points[tri[1]]) * ratio_bc;
    Point zero_ca =
        v_points[tri[0]] + (v_points[tri[2]] - v_points[tri[0]]) * ratio_ca;

    bool a_near_zero = near_zero(iso_a);
    bool b_near_zero = near_zero(iso_b);
    bool c_near_zero = near_zero(iso_c);

    if (a_near_zero) {
      segment_points_.push_back(v_points[tri[0]]);
      if (b_near_zero) {
        segment_points_.push_back(v_points[tri[1]]);
        continue;
      }
      if (c_near_zero) {
        segment_points_.push_back(v_points[tri[2]]);
        continue;
      }

      segment_points_.push_back(zero_bc);
      continue;
    }
    if (b_near_zero) {
      segment_points_.push_back(v_points[tri[1]]);
      if (c_near_zero) {
        segment_points_.push_back(v_points[tri[2]]);
        continue;
      }
      segment_points_.push_back(zero_ca);
      continue;
    }
    if (c_near_zero) {
      segment_points_.push_back(v_points[tri[2]]);
      segment_points_.push_back(zero_ab);
      continue;
    }

    if (iso_a * iso_b > 0) {
      if (iso_a * iso_c > 0) {
        continue;
      } else {
        segment_points_.push_back(zero_ca);
        segment_points_.push_back(zero_bc);
      }
    } else {
      if (iso_a * iso_c > 0) {

        segment_points_.push_back(zero_ab);
        segment_points_.push_back(zero_bc);
      } else {
        segment_points_.push_back(zero_ca);
        segment_points_.push_back(zero_ab);
      }
    }
  }
}
