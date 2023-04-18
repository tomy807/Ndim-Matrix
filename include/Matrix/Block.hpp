// #ifndef BLOCK_H
// #define BLOCK_H

// #include <vector>
// #include <iostream>
// #include <cassert>
// #include <type_traits>
// #include "Matrix.hpp"

// namespace internal {
//     template <typename _Scalar, int _Rows, int _Cols> 
//     class Matrix;

//     template <typename _Scalar, int blockRows,int blockCols>
//     class Block{
//         public:
//             // template <typename _Scalar, int _Rows, int _Cols>
//             // Block(const Matrix<_Scalar,_Rows,_Cols>& other_){
//             //     other_.row();
//             // };

//             template<int _Rows, int _Cols>
//             void setMatrix(const Matrix<_Scalar,_Rows,_Cols>& matrix){
//                 this->matrix_ = matrix;
//             }
//             // template <typename _Scalar, int blockRows,int blockCols>
//             friend std::ostream& operator<<(std::ostream& os, const Block& m){
//             int rows = blockRows;
//             int cols = blockCols;

//             for(int i = 0; i < rows; i++) {
//                 for(int j = 0; j < cols; j++) {
//                     os << matrix_.data[i*cols + j] << " ";
//                 }
//                 os << "\n";
//             }
//             return os;
//         }

//         Block(int startRow, int startCol){
//             startRow_ = startRow;
//             startCol_ = startCol;
//         };
//         Block(){};
        
//         private:
//             Matrix<_Scalar,_Rows,_Cols> matrix_; 
//             int startRow_;
//             int startCol_;
//     };
// }

// #endif