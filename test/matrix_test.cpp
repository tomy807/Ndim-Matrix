#include <gtest/gtest.h>
#include "../include/Matrix/test_matrix.hpp"
#include "../include/Decompose/Decompose.hpp"
Matrix<float> matrix({{1.0,2.0,3.0},{0.0,1.0,4.0},{5.0,6.0,0.0}});


TEST(MatrixTest, Constructor) {
  Matrix<double> A(2, 3);
  EXPECT_EQ(A.rows(), 2);
  EXPECT_EQ(A.cols(), 3);

  Matrix<double> B(3, 2);
  EXPECT_EQ(B.rows(), 3);
  EXPECT_EQ(B.cols(), 2);
  EXPECT_DOUBLE_EQ(B(0, 0), 0.0);
  EXPECT_DOUBLE_EQ(B(1, 1), 0.0);
}

TEST(MatrixTest,Move_Constructor){
  Matrix<int> A(3,3);
  Matrix<int> B(A);
  Matrix<int> C = A+B;
  A.print();
  B.print();
  C.print();
}

TEST(MatrixTest,Print){
  matrix.print();
}

TEST(MatrixTest, Operator) {
  Matrix<int> A(2, 3);
  A(0, 0) = 1;
  A(0, 1) = 2;
  A(0, 2) = 3;
  A(1, 0) = 4;
  A(1, 1) = 5;
  A(1, 2) = 6;

  EXPECT_EQ(A(0, 0), 1);
  EXPECT_EQ(A(0, 1), 2);
  EXPECT_EQ(A(0, 2), 3);
  EXPECT_EQ(A(1, 0), 4);
  EXPECT_EQ(A(1, 1), 5);
  EXPECT_EQ(A(1, 2), 6);
}

TEST(MatrixTest, Add_Scalar){
  auto Add_a = matrix+1.;
  Add_a.print();
}

TEST(MatrixTest, Sub_Scalar){
  auto Sub_a = matrix-1.;
  Sub_a.print();
}

TEST(MatrixTest, Mul_Scalar){
  auto Mul_a = matrix*2.;
  Mul_a.print();
}

TEST(MatrixTest,LU_Matrix){
  Matrix<float> L, U;
  Decompose<float>::decomposeLU2(matrix, L, U);
  // matrix.decomposeLU(L,U);
  L.print();
  U.print();
  auto det = matrix.determinant();
  std::cout << det << std::endl;
}

TEST(MatrixTest,Inverse){
  std::cout <<matrix.determinant() << std::endl;
  matrix.inverse().print();
  (matrix*matrix.inverse()).print();
}

