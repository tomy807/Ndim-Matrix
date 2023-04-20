#include <gtest/gtest.h>
#include <typeinfo>
#include "matrix/Matrix.hpp"


using namespace internal;
TEST(Matrix, Constructor) {
    Matrix3f a;
    MatrixXf b;
    Matrix<int,7,5> c;
    std::cout << a;
    std::cout << b;
    std::cout << c;
}
TEST(Matrix, Constructor_Fixed) {
    EXPECT_THROW(Matrix3f a(10,15);, std::invalid_argument);
    Matrix3f b;
    EXPECT_EQ(b.rows(), 3);
    EXPECT_EQ(b.cols(), 3);
    std::cout << b;
}

TEST(Matrix, Constructor_Dynamic) {
    MatrixXf a(3,4);
    VectorXf b(3);
    std::cout << a;
    std::cout << b;
}

TEST(Matrix, Vector) {
    VectorXf b(4);
    b << 1,2,3;
    EXPECT_THROW(Vector3f c(4);, std::invalid_argument);
    std::cout << b;
}

TEST(Matrix,RowVector){
    RowVector3i a;
    a << 1,2,3;
    std::cout << a;
}


TEST(Matrix,Constructor_List_Fixed) {
    Matrix2d a({{1,2},{3,4}});
    std::cout << a;

    Matrix2d b {{1,2},{3,4}};
    std::cout << b;

    Matrix<int,2,3> c {{2, 3, 4},{5, 6, 7}};
    std::cout << c;

    EXPECT_THROW((Matrix<int,2,4>({{2, 3, 4},{5, 6, 7}})) , std::invalid_argument);
    EXPECT_THROW((Matrix2i({{2, 3, 4},{5, 6, 7}})) , std::invalid_argument);
    EXPECT_THROW((Matrix2i({{2, 3},{5, 6, 7}})) , std::invalid_argument);
}

TEST(Matrix,Constructor_List_Dynamic) {
    MatrixXi a({{1,2},{3,4}});
    std::cout << a;

    MatrixXi b {{1,2},{3,4}};
    std::cout << b;
    b.cols();

    EXPECT_THROW((MatrixXi({{2, 3},{5, 6, 7}})) , std::invalid_argument);
}

TEST(Matrix,Comma_Cin){
    Matrix3i a;
    a <<1,2,3,4,5,6,7,8,9,10;
    std::cout << a;
    MatrixXi b(2,3);
    b <<1,2,3,4,5,6;
    std::cout << b;
}

TEST(Matrix,Cout){
    Matrix3i a;
    a <<1,2,3,4,5,6,7,8,9;
    std::cout << a;
}

TEST(Matrix,OtherConstruct){
    MatrixXf a(2,2);
    a <<1,2,3,4;
    Matrix2f b(a);
    std::cout << b;
}

TEST(Matrix,SameConstruct){
    Matrix2d a;
    a << 1,2,3,4;
    Matrix2d b(a);
    std::cout << b;
}