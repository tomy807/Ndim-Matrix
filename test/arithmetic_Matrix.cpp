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

TEST(Matrix_Arithemetic,Arithemetic_Dot){
    Matrix3f a;
    a << 1,2,3,4,5,6,7,8,9;
    Matrix3f b;
    b << 2,3,4,5,6,7,8,9,10;
    Matrix3f c;
    c = a*b;
    std::cout << c;

    Matrix<float,2,3> d;
    d << 1,1,1,1,1,1;
    Matrix<float,3,2> e;
    e << 1,1,1,1,1,1;
    Matrix<float,2,2> f;
    f = d*e;
    std::cout <<f;
}

TEST(Matrix_Arithemetic,Arithemetic_Dot_Exception){
    Matrix<float,2,3> d;
    d << 1,1,1,1,1,1;
    Matrix<float,3,2> e;
    e << 1,1,1,1,1,1;
    
    EXPECT_THROW(d*e;, std::invalid_argument);
}
