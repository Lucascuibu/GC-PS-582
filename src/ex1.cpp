#include "ex1.h"

void EigenTutorial::solve_sparse_linear_system()
{
    SMatd A(5, 5); 
    std::vector<T> tripletList;
    VecXd b(5); 

    tripletList.push_back(T(0, 1, 1));
    tripletList.push_back(T(0, 3, -2));
    tripletList.push_back(T(1, 0, -1));
    tripletList.push_back(T(1, 2, 3));
    tripletList.push_back(T(1, 4, 4));
    tripletList.push_back(T(2, 3, 5));
    tripletList.push_back(T(2, 4, 2));
    tripletList.push_back(T(3, 0, -1));
    tripletList.push_back(T(3, 1, 3));
    tripletList.push_back(T(4, 2, 1));
    tripletList.push_back(T(4, 3, 1));
    tripletList.push_back(T(4, 4, 1));

    A.setFromTriplets(tripletList.begin(), tripletList.end());

    A.makeCompressed(); 
    // std::cout << "A = " << A << std::endl;

    b << 1, 10, 8, 0, 3;

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(A);
    if (solver.info() != Eigen::Success)
    {
        return;
    }
    VecXd x = solver.solve(b);
    if (solver.info() != Eigen::Success)
    {
        return;
    }

    // std::cout << "A: " << A << '\n'
    //           << std::endl;
    // std::cout << "b: " << b.transpose() << '\n'
    //           << std::endl;
    std::cout << "x: " << x.transpose() << '\n'
              << std::endl;
}
