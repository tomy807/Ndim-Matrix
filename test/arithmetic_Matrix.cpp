#include <gtest/gtest.h>
#include <typeinfo>
#include "../include/Matrix/Matrix.hpp"

using namespace internal;

TEST(Matrix_Arithemetic,SUM){
    Matrix3f a;
    a << 1,2,3,4,5,6,7,8,9;
    Matrix3f b;
    b << 2,3,4,5,6,7,8,9,10;
    Matrix3f c;
    c = a+b;
    std::cout << c;
}
