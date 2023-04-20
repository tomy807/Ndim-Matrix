#include <gtest/gtest.h>
#include "matrix/Matrix.hpp"

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
  m.block(1,1,1,3);

  for (int i = 1; i <= 3; ++i)
  {
    cout << "Block of size " << i << "x" << i << endl;
    cout << m.block(0,0,i,i) << endl << endl;
  }


}

TEST(Block_Matrix,rvalue){

  Matrix2f m;
  m << 0,0,
       0,0;
  Matrix3f a;
    a <<  1, 2, 3, 4,
        5, 6, 7, 8,
        9;
  std::cout << "Here is the array a:\n" << a << "\n\n";
  a.block<2,2>(1,1) = m;
  std::cout << "Here is now a with m copied into its central 2x2 block:\n" << a << "\n\n";
  a.block(0,0,1,3) = a.block(1,0,1,3);
  std::cout << "Here is now a with bottom-right 1x3 block copied into top-left 1x3 block:\n" << a << "\n\n";
}