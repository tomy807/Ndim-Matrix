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
  cout << m.block<1,3>(1,1);

  cout << m.block(1,1,1,3);
}

