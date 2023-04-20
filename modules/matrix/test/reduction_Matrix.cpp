#include <gtest/gtest.h>
#include "matrix/Matrix.hpp"
#include <time.h>
// clock_t start, finish;
// double duration;
// start = clock();
// finish = clock();
// duration = (double)(finish - start) / CLOCKS_PER_SEC;
// printf("%f초", duration);

using namespace internal;
TEST(Reduction_Matrix,Sum){
    Matrix2d a;
    a << 1.1,2.2,3.3,4.5;
    std::cout << a << std::endl;
    std::cout << a.sum() << std::endl;
}

TEST(Reduction_Matrix,Prod){
    Matrix2d a;
    a << 1.1,2.2,3.3,4.5;
    std::cout << a << std::endl;
    std::cout << a.prod() << std::endl;
}

TEST(Reduction_Matrix,Mean){
    MatrixXd a(2,2);
    a << 1.1,2.2,3.3,4.5;
    std::cout << a.size() << std::endl;
    std::cout << a.mean() << std::endl;
}

TEST(Reduction_Matrix,MinMax){
    MatrixXd a(2,2);
    a << 1.1,2.2,3.3,4.5;
    std::cout << a.minCoeff() << std::endl;
    std::cout << a.maxCoeff() << std::endl;
}

TEST(Reduction_Matrix,Trace){
    MatrixXd a(2,2);
    a << 1.1,2.2,3.3,4.5;
    std::cout << a.trace() << std::endl;
}

