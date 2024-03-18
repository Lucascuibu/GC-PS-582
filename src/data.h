#ifndef DATA_H
#define DATA_H

#include "ex1.h"
#include "ex2.h"
#include "imgui.h"
#include "polyscope/polyscope.h"
#include "polyscope/view.h"
#include <vector>
#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/vertex_position_geometry.h"

#include "geometrycentral/surface/direction_fields.h"

#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"

#include "args/args.hxx"
#include "imgui.h"

#include <memory> 


extern polyscope::SurfaceMesh *psMesh;
extern std::unique_ptr<ManifoldSurfaceMesh> mesh;
extern std::unique_ptr<VertexPositionGeometry> geometry;
extern std::unique_ptr<IsoContouring> ic;
void initializeGlobalData();


#endif // DATA_H