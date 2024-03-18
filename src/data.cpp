#include "data.h"

// 定义全局变量
polyscope::SurfaceMesh *psMesh = nullptr;
std::unique_ptr<ManifoldSurfaceMesh> mesh = nullptr;
std::unique_ptr<VertexPositionGeometry> geometry = nullptr;
