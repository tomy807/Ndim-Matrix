#ifndef TEST_TEMPLATE_MATRIX_2_H
#define TEST_TEMPLATE_MATRIX_2_H

#include <vector>
#include <iostream>
#include <cassert>

namespace internal {
    template<typename _Scalar, int _Rows, int _Cols>
    class Matrix {
    public:
        struct Loader{
            Matrix& m;
            int i;
            Loader(Matrix& m, int i) : m(m), i(i) {}
            Loader operator , (_Scalar x) {
                m.data[i] = x;
                return Loader(m, i+1);
            }
        };
        // if Fixed: Assigned But Dynamic: Not Assigned
        Matrix() {
            if(!isDynamic(_Rows, _Cols)){
                data_cols = _Cols;
                data_rows = _Rows;
                data.resize(data_cols*data_rows);
                for (int i = 0; i < data_cols * data_rows; ++i)
                    data[i] = 0;
            }
        };
        
        // if Fixed Not Assigned But Must Dynamic
        Matrix(const int rows,const int cols) {
            if(_Rows != -1 || _Cols != -1){
                throw std::invalid_argument("Invalid matrix definition");
            }
            assert(rows >= 0 && cols >= 0);
            data_cols = cols;
            data_rows = rows;
            data.resize(data_cols * data_rows);
        };

        // For Vector
        Matrix(const int cols) {
            std::cout << "cols: "<< _Cols << "rows: " << _Rows << std::endl;
            if(_Rows != -1){
                throw std::invalid_argument("Invalid matrix definition");
            }
            assert(cols >= 0);
            data_cols = cols;
            data.resize(data_cols);
            for (int i = 0; i < data_cols; ++i)
                    data[i] = 0;
        };

        Matrix(std::initializer_list<std::initializer_list<_Scalar>> list) {
            int list_rows = list.size();
            int list_cols = list.begin()->size();
            
            // Fixed
            if(_Rows != -1 || _Cols != -1){
                // Check Fixed Size == List Size
                if(_Rows != list_rows || _Cols != list_cols){
                    throw std::invalid_argument("Invalid matrix definition");
                }
                setDataByLists(_Rows, _Cols,list);
            // Dynamic
            }else{
                setDataByLists(list_rows, list_cols,list);
            }
        }


        Matrix(const Matrix& other) {
            for (int i = 0; i < _Rows * _Cols; ++i)
                data[i] = other.data[i];
        };

        Matrix(const std::vector<_Scalar>& values) {
            assert(values.size() == _Rows * _Cols);
            for (int i = 0; i < _Rows * _Cols; ++i)
                data[i] = values[i];
        };
        ~Matrix() {};

        Loader operator<<(_Scalar value) {
            if(isDynamic(_Rows,_Cols)){
                throw std::invalid_argument("Invalid matrix operator");
            }
            data[0] = value;
            return Loader(*this,1);
        }

        void print() const {
            if(data_rows==-1){
                for(int j = 0; j < data_cols; j++) {
                    std::cout << data[j] << " ";
                }
                std::cout << "\n";
            }else{
                for(int i = 0; i < data_rows; i++) {
                    for(int j = 0; j < data_cols; j++) {
                        std::cout << data[i*data_cols + j] << " ";
                    }
                    std::cout << "\n";
                }
            }
        }

        bool isDynamic(int rows,int cols) const { 
            if(rows==-1 && cols==-1) 
            {
                return true;
            }
            return false;
        }

        int rows() const{
            return data_rows;
        }

        int cols() const{
            return data_cols;
        }

        void setDataByLists(int list_rows,int list_cols,std::initializer_list<std::initializer_list<_Scalar>>& list){
            data_cols = list_cols;
            data_rows = list_rows;
            data.resize(data_cols * data_rows);
            int i, j;
            i = j = 0;
            for(auto outer : list) {
                if(outer.size() != data_cols){
                    throw std::invalid_argument("Invalid matrix definition");
                }
                for(auto inner : outer) {
                    data[i*data_cols+j++] = inner;
                }
                j = 0;
                i++;
            }
        }
    private:
        std::vector<_Scalar> data;
        int data_cols=-1;
        int data_rows=-1;
    };
    const int Dynamic = -1;

#define EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, Size, SizeSuffix) \
    typedef Matrix<Type, Size, Size> Matrix##SizeSuffix##TypeSuffix; \
    typedef Matrix<Type, Size, 1> Vector##SizeSuffix##TypeSuffix; \
    typedef Matrix<Type, 1, Size> RowVector##SizeSuffix##TypeSuffix;

#define EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, Size) \
    typedef Matrix<Type, Size, Dynamic> Matrix##Size##X##TypeSuffix; \
    typedef Matrix<Type, Dynamic, Size> Matrix##X##Size##TypeSuffix;

#define EIGEN_MAKE_TYPEDEFS_ALL_SIZES(Type, TypeSuffix) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 2, 2) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 3, 3) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 4, 4) \
    EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, Dynamic, X) \
    EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 2) \
    EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 3) \
    EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 4)
    EIGEN_MAKE_TYPEDEFS_ALL_SIZES(int, i)
    EIGEN_MAKE_TYPEDEFS_ALL_SIZES(float, f)
    EIGEN_MAKE_TYPEDEFS_ALL_SIZES(double, d)

#undef EIGEN_MAKE_TYPEDEFS_ALL_SIZES
#undef EIGEN_MAKE_TYPEDEFS
#undef EIGEN_MAKE_FIXED_TYPEDEFS
}
#endif
