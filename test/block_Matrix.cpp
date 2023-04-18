#include <gtest/gtest.h>
#include "../include/Matrix/Matrix.hpp"

using namespace internal;
using namespace std;

TEST(Block_Matrix,Block_Construct){
Matrix4f m;
  m <<  1, 2, 3, 4,
        5, 6, 7, 8,
        9,10,11,12,
       13,14,15,16;
  cout << "Block in the middle" << endl;
  cout << m << endl;
  cout << m.block<1,3>(1,1);
  // m.block(1,1,1,3);

  // for (int i = 1; i <= 3; ++i)
  // {
  //   cout << "Block of size " << i << "x" << i << endl;
  //   cout << m.block(0,0,i,i) << endl << endl;
  // }


}

TEST(Block_Matrix,rvalue){

  Matrix2f m;
  m << 2,2,
       2,2;
  Matrix4f a = Matrix4f::Constant(1);
  std::cout << "Here is the array a:\n" << a << "\n\n";
  a.block<2,2>(1,1) = m;
  std::cout << "Here is now a with m copied into its central 2x2 block:\n" << a << "\n\n";
  a.block(0,0,2,3) = a.block(2,1,2,3);
  std::cout << "Here is now a with bottom-right 2x3 block copied into top-left 2x3 block:\n" << a << "\n\n";
}