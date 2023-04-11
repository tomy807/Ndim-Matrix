#ifndef TEST_TEMPLATE_MATRIX_H
#define TEST_TEMPLATE_MATRIX_H

#include <vector>
#include <iostream>
#include <cassert>
#include <type_traits>

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
        // Fixed
        Matrix() {
            if(setOption(_Rows, _Cols)==FIXED){
                data_cols = _Cols;
                data_rows = _Rows;
                data.resize(data_cols*data_rows);
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

        // Fixed
        static Matrix Random(){
            if(_Rows == -1 || _Cols == -1){
                throw std::invalid_argument("Invalid Matrix definition");
            }
            Matrix result;
            if(std::is_same<_Scalar, int>::value){
                for(int i=0; i<result.size(); ++i){
                    result.setElement(i,rand());
                }
            }else{
                for(int i=0; i<result.size(); ++i){
                    result.setElement(i,-1+2*_Scalar(rand())/_Scalar(RAND_MAX));
                }
            }
            return result;
        };

        static Matrix Random(int row_or_col) {
            assert(row_or_col >= 0);
            if(_Rows == -1 || _Cols == -1){
                throw std::invalid_argument("Invalid Matrix definition");
            }
            Matrix result;
            if(std::is_same<_Scalar, int>::value){
                for(int i=0; i<result.size(); ++i){
                    result.setElement(i,rand());
                }
            }else{
                for(int i=0; i<result.size(); ++i){
                    result.setElement(i,-1+2*_Scalar(rand())/_Scalar(RAND_MAX));
                }
            }
            return result;
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
        
        // Matrix<_Scalar,_Cols,_Rows> transpose(){

        template<uint blockRows,uint blockCols>
        Matrix<_Scalar,blockRows,blockCols> block(const uint startRow,const uint startCol){
            if(startRow+blockRows>rows() || startCol+blockCols>cols()){
                throw std::invalid_argument("Invalid Block size");
            }
            Matrix<_Scalar,blockRows,blockCols> result;
            int index = 0;
            for (int i = startRow;i<startRow+blockRows;i++){
                for (int j = startCol; j<startCol+blockCols;j++){
                    result.setElement(index,getElement(i,j));
                    index++;
                }
            }
            return result; 
        }

        Matrix<_Scalar,Dynamic,Dynamic> block(const int startRow,const int startCol,const int blockRow,const int blockCol){
            if(startRow+blockRow>rows() || startCol+blockCol>cols()){
                throw std::invalid_argument("Invalid Block size");
            }
            Matrix<_Scalar,Dynamic,Dynamic> result(blockRow,blockCol);
            int index = 0;
            for (int i = startRow;i<startRow+blockRow;i++){
                for (int j = startCol; j<startCol+blockCol;j++){
                    result.setElement(index,getElement(i,j));
                    index++;
                }
            }
            return result; 
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

        const _Scalar sum() const {
            _Scalar sum = static_cast<_Scalar>(0.0);
            if(data.empty()){
                return sum;
            }
            for(auto i: data){
                sum+=i;
            }
            return sum;
        }

        const _Scalar prod() const {
            _Scalar prod = static_cast<_Scalar>(1.0);
            if(data.empty()){
                return prod;
            }
            for(auto i: data){
                prod*=i;
            }
            return prod;
        }

        const _Scalar mean() const {
            if(data.empty()){
                return static_cast<_Scalar>(0.0);
            }
            return sum()/size();
        }

        const _Scalar minCoeff() const {
            _Scalar min_val = data[0];
            for(auto i: data){
                if(i<min_val){
                    min_val=i;
                }
            }
            return min_val;
        }

        _Scalar maxCoeff() const {
            _Scalar max_val = data[0];
            for(auto i: data){
                if(i>max_val){
                    max_val=i;
                }
            }
            return max_val;
        }

        _Scalar trace() const {
            if(data_cols != data_rows){
                throw std::invalid_argument("Invalid Trace Matrix");
            }
            _Scalar trace = static_cast<_Scalar>(0.0);
            if(data.empty()){
                return trace;
            }
            for (int i = 0; i < data_rows; i++){
                trace += getElement(i,i);
            }
            return trace;
            
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
        
        _Scalar getElement(const int row, const int col) const{
            return data[row*data_cols+col];
        }
        
        int size() const { 
            return data.size();
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

#define MAKE_TYPEDEFS(Type, TypeSuffix, Size, SizeSuffix) \
    typedef Matrix<Type, Size, Size> Matrix##SizeSuffix##TypeSuffix; \
    typedef Matrix<Type, Size, 1> Vector##SizeSuffix##TypeSuffix; \
    typedef Matrix<Type, 1, Size> RowVector##SizeSuffix##TypeSuffix;

#define MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, Size) \
    typedef Matrix<Type, Size, Dynamic> Matrix##Size##X##TypeSuffix; \
    typedef Matrix<Type, Dynamic, Size> Matrix##X##Size##TypeSuffix;

#define MAKE_TYPEDEFS_ALL_SIZES(Type, TypeSuffix) \
    MAKE_TYPEDEFS(Type, TypeSuffix, 2, 2) \
    MAKE_TYPEDEFS(Type, TypeSuffix, 3, 3) \
    MAKE_TYPEDEFS(Type, TypeSuffix, 4, 4) \
    MAKE_TYPEDEFS(Type, TypeSuffix, Dynamic, X) \
    MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 2) \
    MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 3) \
    MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 4)
    MAKE_TYPEDEFS_ALL_SIZES(int, i)
    MAKE_TYPEDEFS_ALL_SIZES(float, f)
    MAKE_TYPEDEFS_ALL_SIZES(double, d)

#undef MAKE_TYPEDEFS_ALL_SIZES
#undef MAKE_TYPEDEFS
#undef MAKE_FIXED_TYPEDEFS
}
#endif
