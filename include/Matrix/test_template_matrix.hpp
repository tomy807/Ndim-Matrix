#ifndef TEST_TEMPLATE_MATRIX_H
#define TEST_TEMPLATE_MATRIX_H
#include <vector>
#include <iostream>
#include <cassert>
// #include "../Decompose/Decompose.hpp"

// template<typename T>
// class Decompose;

template<typename Scalar>
class Data{
    private:
    int rows_;
    int cols_;
    std::vector<Scalar> data_;
    public:
    void set_data(const int rows, const int cols){
        rows_ = rows; cols_ = cols;
        data_.resize(rows*cols);
    }
};

template<typename Scalar, int Rows, int Cols>
class Matrix {
public:
    // Constructor
    Matrix() : data_() {
        std::cout << "Default constructor" << std::endl;
    }
    Matrix(int rows, int cols){
        if(Rows==-1 && Cols==-1){
            data_test.set_data(rows,cols);
        }else{
            throw std::invalid_argument("Invalid Matrix Resize");
        }
    }
    
    // Destructor
    ~Matrix() = default;
    
    // Copy constructor
    Matrix(const Matrix& other) {
        for (int i = 0; i < Rows * Cols; i++) {
            this->data_[i] = other.data_[i];
        }
    }
    
    // Move constructor
    Matrix(Matrix&& other) noexcept {
        for (int i = 0; i < Rows * Cols; i++) {
            this->data_[i] = std::move(other.data_[i]);
        }
    }

    Matrix(std::initializer_list<std::initializer_list<Scalar>> list) {
        
        int i, j;
        i = j = 0;
        for(auto outer : list) {
            if(outer.size() != Cols)
                throw std::invalid_argument("Invalid matrix definition");
            for(auto inner : outer) {
                data_[i*Cols+j++] = inner;
            }
            j = 0;
            i++;
        }
    }
    
    // Copy assignment operator
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            for (int i = 0; i < Rows * Cols; i++) {
                this->data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    // Move assignment operator
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            for (int i = 0; i < Rows * Cols; i++) {
                this->data_[i] = std::move(other.data_[i]);
            }
        }
        return *this;
    }
    

    
    // Access element at row i and column j
    Scalar& operator()(int i, int j) {
        return this->data_[i * Cols + j];
    }
    
    // Access element at row i and column j (const)
    const Scalar& operator()(int i, int j) const {
        return this->data_[i * Cols + j];
    }

        // Get number of rows
    int rows() const {
        return Rows;
    }

    // Get number of columns
    int cols() const {
        return Cols;
    }
    void resize(int rows, int cols) {
        if(Cols ==-1 && Rows ==-1){
            data_test.set_data(rows,cols);
        }else{
            throw std::invalid_argument("Invalid Matrix Resize");
        }

    }

    void print() {
            for(int i = 0; i < Rows; i++) {
                for(int j = 0; j < Cols; j++) {
                    std::cout << data_[i*Cols + j] << " ";
                }
                std::cout << "\n";
            }
    }
    
    private:
        // Private member variables
        // Data<Scalar> data_;
        Scalar data_[Rows * Cols];
        Data<Scalar> data_test;

};

const int Dynamic = -1;

#define EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, Size, SizeSuffix)   \
    typedef Matrix<Type, Size, Size> Matrix##SizeSuffix##TypeSuffix;  \
    typedef Matrix<Type, Size, 1>    Vector##SizeSuffix##TypeSuffix;  \
    typedef Matrix<Type, 1, Size>    RowVector##SizeSuffix##TypeSuffix;

#define EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, Size)         \
    typedef Matrix<Type, Size, Dynamic> Matrix##Size##X##TypeSuffix;  \
    typedef Matrix<Type, Dynamic, Size> Matrix##X##Size##TypeSuffix;

#define EIGEN_MAKE_TYPEDEFS_ALL_SIZES(Type, TypeSuffix) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 2, 2) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 3, 3) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 4, 4) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, Dynamic, X) \
    EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 2) \
    EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 3) \
    EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 4)
    EIGEN_MAKE_TYPEDEFS_ALL_SIZES(int,                  i)
    EIGEN_MAKE_TYPEDEFS_ALL_SIZES(float,                f)
    EIGEN_MAKE_TYPEDEFS_ALL_SIZES(double,               d)

#undef EIGEN_MAKE_TYPEDEFS_ALL_SIZES
#undef EIGEN_MAKE_TYPEDEFS
#undef EIGEN_MAKE_FIXED_TYPEDEFS


#endif