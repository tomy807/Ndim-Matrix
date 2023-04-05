#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H
#include <vector>
#include <iostream>
#include <cassert>
#include "../Decompose/Decompose.hpp"

template<typename T>
class Decompose;

template<typename T>
class Matrix {
public:
    // 생성자
    Matrix(){}
    Matrix(int r, int c) : m_rows(r), m_cols(c), m_data(r * c) {}

    // 복사 생성자
    Matrix(const Matrix& other) : m_rows(other.m_rows), m_cols(other.m_cols), m_data(other.m_data) {}

    // 이동 생성자
    Matrix(Matrix&& other) noexcept {
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_data = std::move(other.m_data);
    }

    // List constructor
    Matrix(std::initializer_list<std::initializer_list<T>> list) {
        m_rows = list.size();
        m_cols = list.begin()->size();
        m_data.resize(m_rows*m_cols);

        int i, j;
        i = j = 0;
        for(auto outer : list) {
            if(outer.size() != m_cols)
                throw std::invalid_argument("Invalid matrix definition");
            for(auto inner : outer) {
                m_data[i*m_cols+j++] = inner;
            }
            j = 0;
            i++;
        }
    }

    // 대입 연산자
    Matrix& operator=(const Matrix& other) {
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_data = other.m_data;
        return *this;
    }
    // 이동 연산자
    Matrix& operator=(Matrix&& other) noexcept {
        if(this != &other) {
            m_rows = other.m_rows;
            m_cols = other.m_cols;
            m_data = std::move(other.m_data);
        }
        return *this;
    }


    // 소멸자
    ~Matrix() {}

    // 행렬 크기를 반환하는 함수
    int rows() const { return m_rows; }
    int cols() const { return m_cols; }

    // (i, j)번째 원소에 접근하는 연산자
    T& operator()(int i, int j) { return m_data[i * m_cols + j]; }
    const T& operator()(int i, int j) const { return m_data[i * m_cols + j]; }

    // 덧셈 연산자
    Matrix operator+(const Matrix& other) const {
        assert(m_rows == other.m_rows && m_cols == other.m_cols);
        Matrix result(m_rows, m_cols);
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }
    

    // 뺄셈 연산자
    Matrix operator-(const Matrix& other) const {
        assert(m_rows == other.m_rows && m_cols == other.m_cols);
        Matrix result(m_rows, m_cols);
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }
        return result;
    }

    // 곱셈 연산자
    Matrix operator*(const Matrix& other) const {
        assert(m_cols == other.m_rows);
        Matrix result(m_rows, other.m_cols);
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < other.m_cols; j++) {
                T sum = 0;
                for (int k = 0; k < m_cols; k++) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }


    // Scalar Add
    Matrix operator+(const T& scalar) const {
        Matrix result(m_rows, m_cols);
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                result(i, j) = (*this)(i, j) + scalar;
            }
        }
        return result;
    }
    // Scalar Sub
    Matrix operator-(const T& scalar) const {
        Matrix result(m_rows, m_cols);
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                result(i, j) = (*this)(i, j) - scalar;
            }
        }
        return result;
    }
    // Scalar Mul
    Matrix operator*(const T& scalar) const {
        Matrix result(m_rows, m_cols);
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                result(i, j) = (*this)(i, j) * scalar;
            }
        }
        return result;
    }
    

    // LU 분해를 수행하는 함수
    void decomposeLU(Matrix& L, Matrix& U) const {
        assert(m_rows == m_cols);
        const int n = m_rows;

        // L과 U를 초기화한다.
        L = Matrix(n, n);
        U = Matrix(n, n);

        // U의 첫 번째 행을 A의 첫 번째 행으로 설정한다.
        for (int j = 0; j < n; j++) {
            U(0, j) =(*this)(0, j);
        }
        for (int i = 1; i < n; i++) {
            L(i, 0) = (*this)(i, 0) / U(0, 0);
        }
        for (int i = 1; i < n; i++) {
                for (int j = i; j < n; j++) {
                    // U의 i번째 행 계산
                    T sum = 0;
                    for (int k = 0; k < i; k++) {
                        sum += L(i, k) * U(k, j);
                    }
                    U(i, j) = (*this)(i, j) - sum;

                    // L의 j번째 열 계산
                    if (j == i) {
                        L(i, j) = 1;
                    } else {
                        sum = 0;
                        for (int k = 0; k < i; k++) {
                            sum += L(j, k) * U(k, i);
                        }
                        L(j, i) = ((*this)(j, i) - sum) / U(i, i);
                    }
                }
            }
    }

// 역행렬을 계산하는 함수
    Matrix inverse() const {
        assert(m_rows == m_cols);
        const int n = m_rows;

        // LU 분해를 수행한다.
        Matrix<T> L, U;
        Matrix<T> result(*this);
        // decomposeLU(L, U);
        Decompose<T>::decomposeLU2(result,L, U);

        // U^-1과 L^-1을 계산한다.
        Matrix Uinv(n, n);
        for (int j = 0; j < n; j++) {
            Uinv(j, j) = 1 / U(j, j);
            for (int i = j - 1; i >= 0; i--) {
                T sum = 0;
                for (int k = i + 1; k <= j; k++) {
                    sum += U(i, k) * Uinv(k, j);
                }
                Uinv(i, j) = -sum / U(i, i);
            }
        }
        Matrix Linv(n, n);
        for (int i = 0; i < n; i++) {
            Linv(i, i) = 1;
            for (int j = i + 1; j < n; j++) {
                T sum = 0;
                for (int k = i; k < j; k++) {
                    sum += L(j, k) * Linv(k, i);
                }
                Linv(j, i) = -sum;
            }
        }

        // A^-1 = U^-1 * L^-1을 계산한다.
        Matrix Ainv(n, n);
        Ainv = Uinv * Linv;
        return Ainv;
    }

    double determinant() {

        assert(m_rows== m_cols);
        const int n = m_rows;

        Matrix B = *this;  // 가우스 소거법을 수행할 복사본
        double det = 1;  // 행렬식의 초기값
        int sign = 1;  // 행 교환의 부호
        // 가우스 소거법으로 삼각행렬을 만든다
        for (int i = 0; i < n; i++) {
            // 행 교환
            if (B(i, i) == 0) {
                int k;
                for (k = i + 1; k < n; k++) {
                    if (B(k, i) != 0) {
                        break;
                    }
                }
                if (k == n) {  // 모든 원소가 0인 경우
                    return 0;
                }
                for (int j = i; j < n; j++) {
                    std::swap(B(i, j), B(k, j));
                }
                sign *= -1;
            }
            // 대각선 원소를 1로 만들기
            det *= B(i, i);
            for (int j = i + 1; j < n; j++) {
                double factor = B(j, i) / B(i, i);
                for (int k = i; k < n; k++) {
                    B(j, k) -= factor * B(i, k);
                }
            }
        }
    // 행렬식 계산
        return sign * det;
    }

    void print() {
            for(int i = 0; i < m_rows; i++) {
                for(int j = 0; j < m_cols; j++) {
                    std::cout << m_data[i*m_cols + j] << " ";
                }
                std::cout << "\n";
            }
    }
    private:
        int m_rows; // 행의 개수
        int m_cols; // 열의 개수
        std::vector<T> m_data; // 행렬의 데이터
};
#endif