#ifndef DECOMPOSE_H
#define DECOMPOSE_H
#include "../Matrix/test_matrix.hpp"

template<typename T>
class Matrix;

template<typename T>
class Decompose {
public:
    static void decomposeLU2(Matrix<T>& target ,Matrix<T>& L, Matrix<T>& U){
        assert(target.rows() == target.cols());
        const int n = target.rows();

        // L과 U를 초기화한다.
        L = Matrix<T>(n, n);
        U = Matrix<T>(n, n);

        // U의 첫 번째 행을 A의 첫 번째 행으로 설정한다.
        for (int j = 0; j < n; j++) {
            U(0, j) =target(0, j);
        }
        for (int i = 1; i < n; i++) {
            L(i, 0) = target(i, 0) / U(0, 0);
        }
        for (int i = 1; i < n; i++) {
                for (int j = i; j < n; j++) {
                    // U의 i번째 행 계산
                    T sum = 0;
                    for (int k = 0; k < i; k++) {
                        sum += L(i, k) * U(k, j);
                    }
                    U(i, j) = target(i, j) - sum;

                    // L의 j번째 열 계산
                    if (j == i) {
                        L(i, j) = 1;
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