#include <gtest/gtest.h>
#include <typeinfo>
#include "../include/Matrix/Matrix.hpp"


using namespace internal;
TEST(Matrix, Constructor) {
    Matrix3f a;
    MatrixXf b;
    Matrix<int,7,5> c;
    a.print();
    b.print();
    c.print();
}
TEST(Matrix, Constructor_Fixed) {
    EXPECT_THROW(Matrix3f a(10,15);, std::invalid_argument);
    Matrix3f b;
    EXPECT_EQ(b.rows(), 3);
    EXPECT_EQ(b.cols(), 3);
    b.print();
}

TEST(Matrix, Constructor_Dynamic) {
    MatrixXf a(10,15);
    VectorXf b(30);
    a.print();
    b.print();
}

TEST(Matrix, Vector) {
    VectorXf b(30);
    EXPECT_THROW(Vector3f c(4);, std::invalid_argument);
    b.print();
}

TEST(Matrix,Constructor_List_Fixed){
    Matrix2d a({{1,2},{3,4}});
    a.print();

    Matrix2d b {{1,2},{3,4}};
    b.print();
}