#include <gtest/gtest.h>
#include "../include/test_matrix.hpp"

TEST(MatrixTest, Constructor) {
  Matrix<int> A(2, 3);
  EXPECT_EQ(A.rows(), 2);
  EXPECT_EQ(A.cols(), 3);

  Matrix<double> B(3, 2);
  EXPECT_EQ(B.rows(), 3);
  EXPECT_EQ(B.cols(), 2);
  EXPECT_DOUBLE_EQ(B(0, 0), 1.5);
  EXPECT_DOUBLE_EQ(B(1, 1), 1.5);
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

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
