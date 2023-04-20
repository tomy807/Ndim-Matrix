#include <gtest/gtest.h>
#include "matrix/Matrix.hpp"
// #include "../include/Decompose/Decompose.hpp"

using namespace internal;
TEST(Matrix, Constructor) {
    Matrix2f a;
    Matrix2f b;
    Matrix2f c;
    a << 1,2,3,4;
    std::cout << a << std::endl;
    // Decompose::decomposeLU(a,b,c);

    std::cout << b << std::endl;
    std::cout << c << std::endl;
    std::cout << b*c << std::endl;
}