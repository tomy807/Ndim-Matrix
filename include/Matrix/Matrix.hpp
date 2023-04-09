#ifndef TEST_TEMPLATE_MATRIX_H
#define TEST_TEMPLATE_MATRIX_H

#include <vector>
#include <iostream>
#include <cassert>
#include <iostream>

namespace internal {
    const int Dynamic = -1;
    enum Options{
        //  -1 , -1
        DYNAMIC,
        // ROWS, -1 
        COL_DYNAMIC,
        // -1 COLS
        ROW_DYNAMIC,
        // ROWS, COLS
        FIXED
    };
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
    public:
        // Fixed
        Matrix() {
            if(setOption(_Rows, _Cols)==FIXED){
                data_cols = _Cols;
                data_rows = _Rows;
                data.resize(data_cols*data_rows);
                for (int i = 0; i < data_cols * data_rows; ++i)
                    data[i] = 0;
            }
        };
        
        // Dynamic
        Matrix(const int rows,const int cols) {
            assert(rows >= 0 && cols >= 0);
            setOption(_Rows, _Cols);
            if(option == FIXED){
                throw std::invalid_argument("Invalid matrix definition");
            }
            data_cols = cols;
            data_rows = rows;
            data.resize(data_cols * data_rows);
        };

        // ROW_Dynamic, COL_Dynamic
        Matrix(const int row_or_col) {
            assert(row_or_col >= 0);
            setOption(_Rows, _Cols);
            if(option==FIXED||option==DYNAMIC){
                throw std::invalid_argument("Invalid matrix definition");
            }else if(option==ROW_DYNAMIC){
                data_cols = _Cols;
                data_rows = row_or_col;
            }else{
                data_cols = row_or_col;
                data_rows = _Rows;
            }
                data.resize(data_cols*data_rows);
            for (int i = 0; i < data_cols*data_rows; ++i)
                    data[i] = 0;
            
        };

        Matrix(std::initializer_list<std::initializer_list<_Scalar>> list) {
            int list_rows = list.size();
            int list_cols = checkListColumns(list);
            
            setOption(_Rows, _Cols);
            checkListMat(list_rows, list_cols);
            setDataByLists(list);
        }

        Matrix(const std::vector<_Scalar>& values) {
            assert(values.size() == _Rows * _Cols);
            for (int i = 0; i < _Rows * _Cols; ++i)
                data[i] = values[i];
        };

        ~Matrix() {};

        Loader operator<<(_Scalar value) {
            data[0] = value;
            return Loader(*this,1);
        }

        friend std::ostream& operator<<(std::ostream& os, const Matrix& m){
            int rows = m.rows();
            int cols = m.cols();
            if(rows==-1){
                for(int j = 0; j < cols; j++) {
                    os << m.data[j] << " ";
                }
                os << "\n";
            }else{
                for(int i = 0; i < rows; i++) {
                    for(int j = 0; j < cols; j++) {
                        os << m.data[i*cols + j] << " ";
                    }
                    os << "\n";
                }
            }
            return os;
        }

        template<int Other_Rows, int Other_Cols>
        Matrix<_Scalar,Dynamic,Dynamic> operator+(const Matrix<_Scalar,Other_Rows,Other_Cols>&  other){
            int lhsCols = cols();
            int lhsRows = rows();
            int rhsCols = other.cols();
            int rhsRows = other.rows();
            if(lhsCols != rhsCols && lhsRows != rhsRows){
                throw std::invalid_argument("Invalid Matrix Operation");
            }
            Matrix<_Scalar,Dynamic,Dynamic> result(rhsRows,rhsCols);
            for(int i=0; i < lhsCols*lhsRows; i++) {
                result.setElement(i,getElement(i)+other.getElement(i));
            }
            return result;
        }

        template<int Other_Rows, int Other_Cols>
        Matrix<_Scalar,Dynamic,Dynamic> operator-(const Matrix<_Scalar,Other_Rows,Other_Cols>&  other){
            int lhsCols = cols();
            int lhsRows = rows();
            int rhsCols = other.cols();
            int rhsRows = other.rows();
            if(lhsCols != rhsCols && lhsRows != rhsRows){
                throw std::invalid_argument("Invalid Matrix Operation");
            }
            Matrix<_Scalar,Dynamic,Dynamic> result(rhsRows,rhsCols);
            for(int i=0; i < lhsCols*lhsRows; i++) {
                result.setElement(i,getElement(i)-other.getElement(i));
            }
            return result;
        }
        
        template<int Other_Rows, int Other_Cols>
        Matrix<_Scalar,Dynamic,Dynamic> operator*(const Matrix<_Scalar,Other_Rows,Other_Cols>& other){
            
            const int leftRows = this->rows();
            const int leftCols = this->cols();
            const int rightRows = other.rows();
            const int rightCols = other.cols();
            if(leftCols != rightRows){
                throw std::invalid_argument("Invalid Matrix Operation");
            }
            Matrix<_Scalar,Dynamic,Dynamic> result(leftRows,rightCols);

            for (int i=0; i<leftRows; i++){
                for (int j=0; j<rightCols; j++){
                    _Scalar sum = static_cast<_Scalar>(0.0);
                    for (int k=0; k<leftCols; k++){
                        sum += this->getElement(i * leftCols + k) * other.getElement(k * rightCols + j);
                    }
                    result.setElement(i * rightCols + j, sum);
                }
            }
            return result;
        }

        template<int Other_Rows, int Other_Cols>
        Matrix<_Scalar,_Rows,_Cols> operator=(const Matrix<_Scalar,Other_Rows,Other_Cols>& other){
            int lhsCols = cols();
            int lhsRows = rows();
            int rhsCols = other.cols();
            int rhsRows = other.rows();
            if(lhsCols != rhsCols && lhsRows != rhsRows){
                throw std::invalid_argument("Invalid Matrix Operation");
            }
            for(int i=0; i < lhsCols*lhsRows; i++) {
                this->setElement(i,other.getElement(i)); 
            }
            return *this;
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

        const Options setOption(int CompileRows,int CompileCols) { 
            if(CompileRows==-1 && CompileCols==-1) {
                option = DYNAMIC;
            }else if(CompileRows!=-1 && CompileCols==-1) {
                option = COL_DYNAMIC;
            }else if(CompileRows==-1 && CompileCols!=-1) {
                option = ROW_DYNAMIC;
            }else{
                option = FIXED;
            }
            return option;
        }

        const int rows() const{
            return data_rows;
        }

        const int cols() const{
            return data_cols;
        }

        void setDataByLists(std::initializer_list<std::initializer_list<_Scalar>>& list){
            int i, j;
            i = j = 0;
            for(auto outer : list) {
                for(auto inner : outer) {
                    data[i*data_cols+j++] = inner;
                }
                j = 0;
                i++;
            }
        }
        
        int checkListColumns(const std::initializer_list<std::initializer_list<_Scalar>>& list) {
            int expectedCols = list.begin()->size();
            for (const auto& row : list) {
                if (row.size() != expectedCols){
                    throw std::invalid_argument("Invalid List");
                }
            }
            return expectedCols; 
        }

        void checkListMat(const int list_rows, const int list_cols){
            if(option==FIXED){
                if(_Rows != list_rows || _Cols != list_cols){
                    throw std::invalid_argument("Invalid List");
                }
                data_rows = _Rows;
                data_cols = _Cols;

            }else if(option==ROW_DYNAMIC){
                if(_Cols != list_cols){
                    throw std::invalid_argument("Invalid List");
                }
                data_cols = _Cols;
                data_rows = list_rows;
            }else if(option==COL_DYNAMIC){
                if(_Rows != list_rows){
                    throw std::invalid_argument("Invalid List");
                }
                data_cols = list_cols;
                data_rows = _Rows;
            }else{
                data_cols = list_cols;
                data_rows = list_rows;
            }
            data.resize(data_cols * data_rows);
        }

        _Scalar getElement(const int index) const{
            return data[index];
        }
        void setElement(int index, _Scalar value){
            data[index] = value;
        }
    private:
        std::vector<_Scalar> data;
        int data_cols=-1;
        int data_rows=-1;
        Options option;
    };

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
