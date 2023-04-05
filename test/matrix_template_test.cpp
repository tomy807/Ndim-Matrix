#include <gtest/gtest.h>
#include <typeinfo>
#include "../include/Matrix/test_template_matrix.hpp"
TEST(MatrixTemplateTest, Constructor) {
    Matrix<float, 2, 3> A;
    A(0,0) = 1.0;
    std::cout <<typeid(A(0,1)).name() <<std::endl;
    EXPECT_EQ(A.rows(), 2);
    EXPECT_EQ(A.cols(), 3);
    A.print();

}

TEST(MatrixTemplateTest, Resize) {
    Matrix<float, 2, 3> A;
    A(0,0) = 1.0;
    EXPECT_THROW(A.resize(3,4);,std::invalid_argument);
    EXPECT_EQ(A.rows(), 2);
    EXPECT_EQ(A.cols(), 3);
    A.print();

}