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

TEST(Matrix,Constructor_List_Fixed) {
    Matrix2d a({{1,2},{3,4}});
    a.print();

    Matrix2d b {{1,2},{3,4}};
    b.print();

    Matrix<int,2,3> c {{2, 3, 4},{5, 6, 7}};
    c.print();

    EXPECT_THROW((Matrix<int,2,4>({{2, 3, 4},{5, 6, 7}})) , std::invalid_argument);
    EXPECT_THROW((Matrix2i({{2, 3, 4},{5, 6, 7}})) , std::invalid_argument);
    EXPECT_THROW((Matrix2i({{2, 3},{5, 6, 7}})) , std::invalid_argument);
}

TEST(Matrix,Constructor_List_Dynamic) {
    MatrixXi a({{1,2},{3,4}});
    a.print();

    MatrixXi b {{1,2},{3,4}};
    b.print();
    b.cols();

    EXPECT_THROW((MatrixXi({{2, 3},{5, 6, 7}})) , std::invalid_argument);
}

TEST(Matrix,Comma_Cin){
    Matrix3i a;
    a <<1,2,3,4,5,6,7,8,9,10;
    a.print();
    MatrixXi b(2,3);
    b <<1,2,3,4,5,6;
    b.print();
}

TEST(Matrix,Cout){
    Matrix3i a;
    a <<1,2,3,4,5,6,7,8,9;
    std::cout << a;
}