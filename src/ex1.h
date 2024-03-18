#ifndef REMB_EIGENTUTORIAL_HH
#define REMB_EIGENTUTORIAL_HH

#include <vector>
#include <iostream>

#include "Eigen/Sparse"
// #include <ObjectTypes/TriangleMesh/TriangleMesh.hh>


class EigenTutorial {
public:
    using SMatd = Eigen::SparseMatrix<double>;
    using T = Eigen::Triplet<double>;
    using VecXd = Eigen::VectorXd;

    EigenTutorial(){}
    ~EigenTutorial(){}

public:
    void solve_sparse_linear_system();
};

#endif 
