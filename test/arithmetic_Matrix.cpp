#include <gtest/gtest.h>
#include <typeinfo>
#include "../include/Matrix/Matrix.hpp"

using namespace internal;

TEST(Matrix_Arithemetic,Fixed_SUM){
    Matrix3f a;
    a << 1,2,3,4,5,6,7,8,9;
    Matrix3f b;
    b << 2,3,4,5,6,7,8,9,10;
    Matrix3f c;
    c = a+b;
    std::cout << c;
}

TEST(Matrix_Arithemetic,Dynamic_SUM){
    MatrixXf a(3,3);
    a << 1,2,3,4,5,6,7,8,9;
    Matrix3f b;
    b << 2,3,4,5,6,7,8,9,10;
    Matrix3f c;
    c = a+b;
    std::cout << c;
}


TEST(Matrix_Arithemetic,Fixed_Dot){
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

TEST(Matrix_Arithemetic,Dynamic_Dot){
    MatrixXi a(2,2);
    a << 1,1,1,1;
    MatrixXi b(2,2);
    b << 1,1,1,1;

    MatrixXi d(2,2);
    d = a*b;
    Matrix2i c;
    c = a*b;

    Matrix2i e = a*b;
    std::cout << c << std::endl;
}

TEST(Matrix_Arithemetic,Arithemetic_Dot_Exception){
    Matrix<float,2,3> d;
    d << 1,1,1,1,1,1;
    Matrix<float,4,2> e;
    e << 1,1,1,1,1,1,1,1;
    
    EXPECT_THROW(d*e, std::invalid_argument);
}
