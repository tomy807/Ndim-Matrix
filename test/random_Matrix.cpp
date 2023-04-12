#include <gtest/gtest.h>
#include "../include/Matrix/Matrix.hpp"

using namespace internal;
using namespace std;
TEST(Random_Matrix,Random_Fixed){
    Matrix2d a = Matrix2d::Random();
    std::cout << a << std::endl;

    Matrix2i b = Matrix2i::Random();
    std::cout << b << std::endl;
}