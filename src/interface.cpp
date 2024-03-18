#include "interface.h"
#include "polyscope/curve_network.h"
using namespace ImGui;

void meCallback() {

  ImGuiStyle &style = GetStyle();

  // 调整圆角大小
  style.WindowRounding = 5.0f; // 窗口圆角
  style.FrameRounding = 3.0f;  // 按钮和其他框架元素的圆角
  style.PopupRounding = 3.0f;  // 弹出窗口的圆角
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
  // style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2f, 0.5f, 0.5f, 1.0f);
  // style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.5f, 0.5f, 1.0f);

  float margin = 10;
  float right_w = 300;

  float w = GetIO().DisplaySize.x;
  float h = GetIO().DisplaySize.y;
  static bool firstTime = true;

  if (firstTime) {
    SetNextWindowPos(ImVec2(w - right_w - margin, margin));
    SetNextWindowSize(ImVec2(right_w, h - 2 * margin));
    firstTime = false;
  }

  static bool open = ImGuiTreeNodeFlags_DefaultOpen;

  ImVec2 full_block_size = ImVec2(284, 25);
  ImVec2 half_block_size = ImVec2(140, 25);
  double spacing = 10;
  double second_offset = half_block_size.x + spacing;

  Begin("CS582", &open);
  PushItemWidth(half_block_size.x);

  Text("Select Target Mesh");
  SameLine(second_offset);
  std::vector<const char *> mesh_list = {"1", "2", "3", "4", "5"};
  static int cur_mesh = 0;
  if (Combo("##mesh", &cur_mesh, mesh_list.data(), mesh_list.size())) {
    std::cout << "cur_mesh: " << mesh_list[0] << std::endl;
  }
  // info 1: Ex1
  if (CollapsingHeader("Ex1: Introduction to Eigen", open)) {
    if (Button("Solve Sparse Linear System", full_block_size)) {
      EigenTutorial et;
      et.solve_sparse_linear_system();
    };
  };

  // info 2: Ex2
  if (CollapsingHeader("Ex2: 2D Marching / Iso Contouring ", open)) {
    static int current_item = 0;
    std::vector<const char *> fuctions = {
        "\\sqrt{x^2+y^2}-1=0",
        "y^2-sin{x^2}-1=0",
        "sin{2x+2y}-cos{4xy}=0",
    };
    AlignTextToFramePadding();

    Text("Function");
    SameLine(second_offset);
    Combo("##", &current_item, fuctions.data(), fuctions.size());
    
    if (Button("Show Iso value and level set", full_block_size)) {
      if(mesh){
        IsoContouring ic (*mesh);
        ic.function_id_ = current_item;
        ic.init_geometry(psMesh->vertexPositions);
        ic.show_isovalue_and_level_set();
        psMesh->addVertexColorQuantity("Vertex Colors", ic.get_color());
        auto s = polyscope::registerCurveNetwork("Line Graphs", ic.vertcies, ic.edges);
      }
    }
  };
  // info 3
  if (CollapsingHeader("Ex3: Smooth 2D Curve", open)) {
    static int ex3_num_points = 30;
    AlignTextToFramePadding();

    // PushItemWidth(half_block_size.x);
    Text("Number of points:"); // 显示标签
    SameLine(second_offset);
    SliderInt("##numPoints", &ex3_num_points, 0, 100);
    Button("Generate random curve", full_block_size);

    static int ex3_iternum = 10;
    static float ex3_eps = 1e-4;
    static int cur_smooth_id = 0;
    const char *smooth_types = "Uniform\0Cotangent\0Mean Value\0";

    Text("Iteration");
    SameLine(second_offset);
    SliderInt("## ", &ex3_iternum, 0, 100);
    Text("Epsilon");
    SameLine(second_offset);
    SliderFloat("##", &ex3_eps, 1e-6, 1e-2);

    Text("Smooth Type");
    SameLine(second_offset);
    if (Combo("##", &cur_smooth_id, smooth_types, 2)) {
      std::cout << "cur_smooth_id: " << cur_smooth_id << std::endl;
    };

    Button("Apply Smooth", full_block_size);
  };

  if (CollapsingHeader("Ex4: Normals And Curvatures", open)) {
    AlignTextToFramePadding();
    static int ex4_normal_type = 0;
    const char *normal_types = "Uniform\0Area Weighted\0Mean Value\0";
    static int ex4_curvature_type = 0;
    const char *curvature_types = "Uniform\0Mean Value\0";

    Text("Normal Type");
    SameLine(second_offset);
    if (Combo("##", &ex4_normal_type, normal_types, 3)) {
      std::cout << "cur_normal: " << ex4_normal_type << std::endl;
    };
    Button("Compute Normals", full_block_size);

    Text("Curvature Type");
    SameLine(second_offset);
    if (Combo("##", &ex4_curvature_type, curvature_types, 2)) {
      std::cout << "cur_curvature: " << ex4_curvature_type << std::endl;
    };
    Button("Compute Curvatures", full_block_size);
  };

  if (CollapsingHeader("Ex5: Delunay Trianglation", open)) {
    AlignTextToFramePadding();
    Button("Generate Basis triangle mesh", full_block_size);
    Button("Reset view", full_block_size);
    Button("Lloyd iteration", full_block_size);
  };

  if (CollapsingHeader("Ex6 Smooth 3D Polygon", open)) {
    AlignTextToFramePadding();
    static int ex6_cur_laplacian = 0;
    const char *laplacian_types = "Uniform\0Cotangent\0Implicit\0";
    static int ex6_iteration = 10;
    static float ex6_time_step = 1e-4;
    static float ex6_feature_enhancement_coefficient = 0.5;

    Text("Operator Type");
    SameLine(second_offset);
    if (Combo("##", &ex6_cur_laplacian, laplacian_types)) {
      std::cout << "cur_laplacian: " << ex6_cur_laplacian << std::endl;
    }

    Button("Apply Smooth", full_block_size);

    Text("Iteration");
    SameLine(second_offset);
    if (SliderInt("##", &ex6_iteration, 0., 100.)) {
      std::cout << "cur_iteration: " << ex6_iteration << std::endl;
    };

    Text("Time step");
    SameLine(second_offset);
    if (SliderFloat("##", &ex6_time_step, 0., 1.)) {
      std::cout << "cur_time_step: " << ex6_time_step << std::endl;
    };

    Text("Feature Enhancement Coefficient");
    SameLine(second_offset);
    if (SliderFloat("##", &ex6_feature_enhancement_coefficient, 0., 1.)) {
      std::cout << "cur_feature_enhancement_coefficient: "
                << ex6_feature_enhancement_coefficient << std::endl;
    };

    if (Button("Enhance Feature", full_block_size)) {
      std::cout << "Enhance Feature" << std::endl;
    };
    PopItemWidth();
  }
  //   if (CollapsingHeader("Ex7: Poisson Reconstruction", open)) {
  //     AlignTextToFramePadding();

  //     static int ex7_octree_depth = 10;

  //     Text("Octree Depth");
  //     SameLine(second_offset);
  //     if (SliderInt("##", &ex7_octree_depth, 0, 100)) {
  //       std::cout << "cur_octree_depth: " << ex7_octree_depth << std::endl;
  //     };

  //     if (Button("Reconstruct", full_block_size)) {
  //       std::cout << "Reconstruct" << std::endl;
  //     };
  //   }
  //   if (CollapsingHeader("Ex8: Remesh 3D Polygon", open)) {
  //     AlignTextToFramePadding();
  //     static int ex8_cur_function = 0;
  //     static int ex8_iteration = 10;
  //     const char *ex8_functions = "Average\0Adaptive\0";
  //     Text("Function");
  //     SameLine(second_offset);
  //     if (Combo("##", &ex8_cur_function, ex8_functions)) {
  //       std::cout << "cur_function: " << ex8_cur_function << std::endl;
  //     };
  //     Text("Iteration");
  //     SameLine(second_offset);
  //     if (SliderInt("##", &ex8_iteration, 0, 100)) {
  //       std::cout << "cur_iteration: " << ex8_iteration << std::endl;
  //     };

  //     static float Target_edge_length_ratio = 0.5;
  //     Text("Target edge length ratio");
  //     SameLine(second_offset);
  //     if (SliderFloat("##", &Target_edge_length_ratio, 0., 1.)) {
  //       std::cout << "cur_Target_edge_length_ratio: " <<
  //       Target_edge_length_ratio
  //                 << std::endl;
  //     };

  //     Button("Remesh", full_block_size);
  //     Button("angential Smoothing", full_block_size);
  //
  //   }

  End();

  //  restore polyscope style, this is ugly
  //   style.WindowRounding = 0.0f;
  //   style.FrameRounding = 0.0f;
  //   style.PopupRounding = 0.0f;
  // style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
  //   style.Colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
}
