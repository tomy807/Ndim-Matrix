#ifndef DECOMPOSE_H
#define DECOMPOSE_H
#include "matrix/Matrix.hpp"

using namespace internal;
// template<typename _Scalar, int _Rows, int _Cols>
// class Matrix;
// template<typename _Scalar, int _Rows, int _Cols>
class Decompose {
public:

    template<typename _Scalar, int _Rows, int _Cols>
    static void decomposeLU(Matrix<_Scalar,_Rows,_Cols>& target ,Matrix<_Scalar,_Rows,_Cols>& L, Matrix<_Scalar,_Rows,_Cols>& U){
        assert(target.rows() == target.cols());
        const int n = target.rows();

        for (int j = 0; j < n; j++) {
            U(0, j) =target(0, j);
        }
        for (int i = 1; i < n; i++) {
            L(i, 0) = target(i, 0) / U(0, 0);
        }
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                _Scalar sum = 0;
                for (int k = 0; k < i; k++) {
                    sum += L(i, k) * U(k, j);
                }
                U(i, j) = target(i, j) - sum;

                if (j == i) {
                    L(i, j) = _Scalar(1);
                } else {
                    sum = 0;
                    for (int k = 0; k < i; k++) {
                        sum += L(j, k) * U(k, i);
                    }
                    L(j, i) = (target(j, i) - sum) / U(i, i);
                }
            }
        }
    }
};

#endif