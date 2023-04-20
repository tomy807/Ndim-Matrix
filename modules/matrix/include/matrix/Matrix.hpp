#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <iostream>
#include <type_traits>
#include <vector>

namespace internal {

template <typename SCALAR, int BLOCKROWS, int BLOCKCOLS>
class Block;

const int Dynamic = -1;
// class Enum으로 바꾸기
enum Options {
    //  -1 , -1
    DYNAMIC,
    // ROWS, -1
    COL_DYNAMIC,
    // -1 COLS
    ROW_DYNAMIC,
    // ROWS, COLS
    FIXED
};
template <typename SCALAR, int ROWS, int COLS>
class Matrix {
   public:
    template <int BLOCKROWS, int BLOCKCOLS>
    class Block {
       public:
        friend std::ostream &operator<<(std::ostream &os, const Block &block) {
            int startC = block.startCol_;
            int startR = block.startRow_;

            for (int i = startC; i < startC + block.blockCols_; i++) {
                for (int j = startR; j < startR + block.blockRows_; j++) {
                    os << block.matrix->getElement(i, j) << " ";
                }
                os << "\n";
            }
            return os;
        }

        Block(int startRow, int startCol, Matrix &result) {
            startRow_ = startRow;
            startCol_ = startCol;
            blockCols_ = BLOCKROWS;
            blockRows_ = BLOCKCOLS;
            matrix = &result;
        };

        Block(int startRow, int startCol, int blockCols, int blockRows,
              Matrix &result) {
            if (blockRows == -1 && blockCols == -1) {
                startRow_ = startRow;
                startCol_ = startCol;
                blockCols_ = blockCols;
                blockRows_ = blockRows;
                matrix = &result;
            } else {
                throw std::invalid_argument("Invalid Block Definition");
            }
        }

        template <int OtherRows, int OtherCols>
        Matrix<SCALAR, ROWS, COLS> operator=(
            const Matrix<SCALAR, OtherRows, OtherCols> &other) {
            int lhsCols = blockCols_;
            int lhsRows = blockRows_;
            int rhsCols = other.cols();
            int rhsRows = other.rows();
            if (lhsCols != rhsCols && lhsRows != rhsRows) {
                throw std::invalid_argument("Invalid Matrix Operation");
            }

            for (int i = 0; i < lhsCols; i++) {
                for (int j = 0; j < lhsRows; j++) {
                    matrix->setElement(i + startCol_, j + startRow_,
                                       other.getElement(i, j));
                }
            }
            return *matrix;
        }

        Matrix operator=(const Block &other) {
            int lhsCols = blockCols_;
            int lhsRows = blockRows_;
            int rhsCols = other.getBlockCols();
            int rhsRows = other.getBlockRows();
            if (lhsCols != rhsCols && lhsRows != rhsRows) {
                throw std::invalid_argument("Invalid Matrix Operation");
            }

            for (int i = 0; i < lhsCols; i++) {
                for (int j = 0; j < lhsRows; j++) {
                    matrix->setElement(
                        i + startCol_, j + startRow_,
                        other.matrix->getElement(i + other.startRow_,
                                                 j + other.startCol_));
                }
            }
            return *matrix;
        }

        ~Block() {}

        int getBlockCols() const { return blockCols_; }
        int getBlockRows() const { return blockRows_; }

       private:
        int startRow_;
        int startCol_;
        int blockCols_;
        int blockRows_;
        Matrix *matrix;
    };

    struct Loader {
        Matrix &m;
        int i;
        Loader(Matrix &m, int i) : m(m), i(i) {}
        Loader operator,(SCALAR x) {
            m.data[i] = x;
            return Loader(m, i + 1);
        }
    };

    Loader operator<<(SCALAR value) {
        data[0] = value;
        return Loader(*this, 1);
    }
    // Fixed
    Matrix() {
        if (setOption(ROWS, COLS) == FIXED) {
            data_cols = COLS;
            data_rows = ROWS;
            data.resize(data_cols * data_rows);
        }
    };

    // Dynamic
    Matrix(const int rows, const int cols) {
        assert(rows >= 0 && cols >= 0);
        setOption(ROWS, COLS);
        if (option == FIXED) {
            throw std::invalid_argument("Invalid matrix definition");
        }
        data_cols = cols;
        data_rows = rows;
        data.resize(data_cols * data_rows);
    };

    // ROW_Dynamic, COL_Dynamic
    Matrix(const int row_or_col) {
        assert(row_or_col >= 0);
        setOption(ROWS, COLS);
        if (option == FIXED || option == DYNAMIC) {
            throw std::invalid_argument("Invalid matrix definition");
        } else if (option == ROW_DYNAMIC) {
            data_cols = COLS;
            data_rows = row_or_col;
        } else {
            data_cols = row_or_col;
            data_rows = ROWS;
        }
        data.resize(data_cols * data_rows);
    };

    Matrix(std::initializer_list<std::initializer_list<SCALAR>> list) {
        int list_rows = list.size();
        int list_cols = checkListColumns(list);

        setOption(ROWS, COLS);
        checkListMat(list_rows, list_cols);
        setDataByLists(list);
    }

    Matrix(const std::vector<SCALAR> &values) {
        assert(values.size() == ROWS * COLS);
        for (int i = 0; i < ROWS * COLS; ++i) data[i] = values[i];
    };

    template <class OtherMatrix>
    Matrix(OtherMatrix &other) {
        data_cols = other.cols();
        data_rows = other.rows();
        data = other.getData();
    }

    template <int OtherROWS, int OtherCOLS>
    Matrix(const Matrix<SCALAR, OtherROWS, OtherCOLS> &&other) {
        data_cols = other.cols();
        data_rows = other.rows();
        data = other.getData();
    }

    Matrix(const Matrix &other) {
        data_cols = other.cols();
        data_rows = other.rows();
        data = other.getData();
    }

    Matrix(Matrix &&other) {
        data_cols = other.cols();
        data_rows = other.rows();
        data = other.getData();
    }

    // Fixed
    static Matrix Random() {
        if (ROWS == -1 || COLS == -1) {
            throw std::invalid_argument("Invalid Matrix definition");
        }
        Matrix result;
        if (std::is_same<SCALAR, int>::value) {
            for (int i = 0; i < result.size(); ++i) {
                result.setElement(i, rand());
            }
        } else {
            for (int i = 0; i < result.size(); ++i) {
                result.setElement(i, SCALAR(-1) + SCALAR(2) * SCALAR(rand()) /
                                                      SCALAR(RAND_MAX));
            }
        }
        return result;
    };

    static Matrix Random(int row_or_col) {
        assert(row_or_col >= 0);
        if (ROWS == -1 || COLS == -1) {
            throw std::invalid_argument("Invalid Matrix definition");
        }
        Matrix result;
        if (std::is_same<SCALAR, int>::value) {
            for (int i = 0; i < result.size(); ++i) {
                result.setElement(i, rand());
            }
        } else {
            for (int i = 0; i < result.size(); ++i) {
                result.setElement(i,
                                  -1 + 2 * SCALAR(rand()) / SCALAR(RAND_MAX));
            }
        }
        return result;
    };

    static Matrix Constant(SCALAR value) {
        if (ROWS == -1 || COLS == -1) {
            throw std::invalid_argument("Invalid Matrix definition");
        }
        Matrix result;
        for (int i = 0; i < result.size(); ++i) {
            result.setElement(i, value);
        }
        return result;
    }

    ~Matrix(){};

    friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
        int rows = m.rows();
        int cols = m.cols();
        if (rows == -1) {
            for (int j = 0; j < cols; j++) {
                os << m.data[j] << " ";
            }
            os << "\n";
        } else {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    os << m.data[i * cols + j] << " ";
                }
                os << "\n";
            }
        }
        return os;
    }

    template <int OtherROWS, int OtherCOLS>
    Matrix operator+(const Matrix<SCALAR, OtherROWS, OtherCOLS> &other) {
        int lhsCols = cols();
        int lhsRows = rows();
        int rhsCols = other.cols();
        int rhsRows = other.rows();
        if (lhsCols != rhsCols && lhsRows != rhsRows) {
            throw std::invalid_argument("Invalid Matrix Operation");
        }
        for (int i = 0; i < lhsCols * lhsRows; i++) {
            setElement(i, getElement(i) + other.getElement(i));
        }
        return *this;
    }

    Matrix operator+(const Matrix &other) {
        int lhsCols = cols();
        int lhsRows = rows();
        int rhsCols = other.cols();
        int rhsRows = other.rows();
        if (lhsCols != rhsCols && lhsRows != rhsRows) {
            throw std::invalid_argument("Invalid Matrix Operation");
        }
        for (int i = 0; i < lhsCols * lhsRows; i++) {
            setElement(i, getElement(i) + other.getElement(i));
        }
        return *this;
    }

    template <int OtherROWS, int OtherCOLS>
    Matrix<SCALAR, Dynamic, Dynamic> operator-(
        const Matrix<SCALAR, OtherROWS, OtherCOLS> &other) {
        int lhsCols = cols();
        int lhsRows = rows();
        int rhsCols = other.cols();
        int rhsRows = other.rows();
        if (lhsCols != rhsCols && lhsRows != rhsRows) {
            throw std::invalid_argument("Invalid Matrix Operation");
        }
        Matrix<SCALAR, Dynamic, Dynamic> result(rhsRows, rhsCols);
        for (int i = 0; i < lhsCols * lhsRows; i++) {
            result.setElement(i, getElement(i) - other.getElement(i));
        }
        return result;
    }

    template <int OtherROWS, int OtherCOLS>
    Matrix<SCALAR, Dynamic, Dynamic> operator*(
        const Matrix<SCALAR, OtherROWS, OtherCOLS> &other) {
        const int leftRows = this->rows();
        const int leftCols = this->cols();
        const int rightRows = other.rows();
        const int rightCols = other.cols();
        if (leftCols != rightRows) {
            throw std::invalid_argument("Invalid Matrix Operation");
        }
        Matrix<SCALAR, Dynamic, Dynamic> result(leftRows, rightCols);

        for (int i = 0; i < leftRows; i++) {
            for (int j = 0; j < rightCols; j++) {
                SCALAR sum = static_cast<SCALAR>(0.0);
                for (int k = 0; k < leftCols; k++) {
                    sum += this->getElement(i * leftCols + k) *
                           other.getElement(k * rightCols + j);
                }
                result.setElement(i * rightCols + j, sum);
            }
        }
        return result;
    }

    template <int OtherROWS, int OtherCOLS>
    Matrix<SCALAR, ROWS, COLS> operator=(
        const Matrix<SCALAR, OtherROWS, OtherCOLS> &other) {
        std::cout << "Operator Different =" << std::endl;
        int lhsCols = cols();
        int lhsRows = rows();
        int rhsCols = other.cols();
        int rhsRows = other.rows();
        if (lhsCols != rhsCols && lhsRows != rhsRows) {
            throw std::invalid_argument("Invalid Matrix Operation");
        }
        for (int i = 0; i < lhsCols * lhsRows; i++) {
            this->setElement(i, other.getElement(i));
        }
        return *this;
    }

    Matrix operator=(const Matrix &other) {
        std::cout << "Operator Same =" << std::endl;
        int lhsCols = cols();
        int lhsRows = rows();
        int rhsCols = other.cols();
        int rhsRows = other.rows();
        if (lhsCols != rhsCols && lhsRows != rhsRows) {
            throw std::invalid_argument("Invalid Matrix Operation");
        }
        for (int i = 0; i < lhsCols * lhsRows; i++) {
            this->setElement(i, other.getElement(i));
        }
        return *this;
    }

    SCALAR operator()(int row, int col) const {
        return data[row * data_cols + col];
    }

    SCALAR &operator()(int row, int col) { return data[row * data_cols + col]; }

    // Matrix<SCALAR,COLS,ROWS> transpose(){

    template <int BLOCKROWS, int BLOCKCOLS>
    Block<BLOCKROWS, BLOCKCOLS> block(int startRow, int startCol) {
        if (startRow + BLOCKROWS > rows() || startCol + BLOCKCOLS > cols()) {
            throw std::invalid_argument("Invalid Block size");
        }
        Block<BLOCKROWS, BLOCKCOLS> result(startRow, startCol, *this);
        return result;
    }

    // template<int BLOCKROWS,int BLOCKCOLS>
    Block<Dynamic, Dynamic> block(int startRow, int startCol,
                                  const int blockRows_, const int blockCols_) {
        if (startRow + blockRows_ > rows() || startCol + blockCols_ > cols()) {
            throw std::invalid_argument("Invalid Block size");
        }
        Block<Dynamic, Dynamic> result(startRow, startCol, blockRows_,
                                       blockCols_, *this);
        return result;
    }

    const Options setOption(int CompileRows, int CompileCols) {
        if (CompileRows == -1 && CompileCols == -1) {
            option = DYNAMIC;
        } else if (CompileRows != -1 && CompileCols == -1) {
            option = COL_DYNAMIC;
        } else if (CompileRows == -1 && CompileCols != -1) {
            option = ROW_DYNAMIC;
        } else {
            option = FIXED;
        }
        return option;
    }

    const int rows() const { return data_rows; }

    const int cols() const { return data_cols; }

    const SCALAR sum() const {
        SCALAR sum = static_cast<SCALAR>(0.0);
        if (data.empty()) {
            return sum;
        }
        for (auto i : data) {
            sum += i;
        }
        return sum;
    }

    const SCALAR prod() const {
        SCALAR prod = static_cast<SCALAR>(1.0);
        if (data.empty()) {
            return prod;
        }
        for (auto i : data) {
            prod *= i;
        }
        return prod;
    }

    const SCALAR mean() const {
        if (data.empty()) {
            return static_cast<SCALAR>(0.0);
        }
        return sum() / size();
    }

    const SCALAR minCoeff() const {
        SCALAR min_val = data[0];
        for (auto i : data) {
            if (i < min_val) {
                min_val = i;
            }
        }
        return min_val;
    }

    SCALAR maxCoeff() const {
        SCALAR max_val = data[0];
        for (auto i : data) {
            if (i > max_val) {
                max_val = i;
            }
        }
        return max_val;
    }

    SCALAR trace() const {
        if (data_cols != data_rows) {
            throw std::invalid_argument("Invalid Trace Matrix");
        }
        SCALAR trace = static_cast<SCALAR>(0.0);
        if (data.empty()) {
            return trace;
        }
        for (int i = 0; i < data_rows; i++) {
            trace += getElement(i, i);
        }
        return trace;
    }

    void setDataByLists(
        std::initializer_list<std::initializer_list<SCALAR>> &list) {
        int i, j;
        i = j = 0;
        for (auto outer : list) {
            for (auto inner : outer) {
                data[i * data_cols + j++] = inner;
            }
            j = 0;
            i++;
        }
    }

    int checkListColumns(
        const std::initializer_list<std::initializer_list<SCALAR>> &list) {
        int expectedCols = list.begin()->size();
        for (const auto &row : list) {
            if (row.size() != expectedCols) {
                throw std::invalid_argument("Invalid List");
            }
        }
        return expectedCols;
    }

    void checkListMat(const int list_rows, const int list_cols) {
        if (option == FIXED) {
            if (ROWS != list_rows || COLS != list_cols) {
                throw std::invalid_argument("Invalid List");
            }
            data_rows = ROWS;
            data_cols = COLS;

        } else if (option == ROW_DYNAMIC) {
            if (COLS != list_cols) {
                throw std::invalid_argument("Invalid List");
            }
            data_cols = COLS;
            data_rows = list_rows;
        } else if (option == COL_DYNAMIC) {
            if (ROWS != list_rows) {
                throw std::invalid_argument("Invalid List");
            }
            data_cols = list_cols;
            data_rows = ROWS;
        } else {
            data_cols = list_cols;
            data_rows = list_rows;
        }
        data.resize(data_cols * data_rows);
    }

    const SCALAR getElement(const int index) const { return data[index]; }

    SCALAR &getElement(const int row, const int col) {
        return data[row * data_cols + col];
    }

    const SCALAR getElement(const int row, const int col) const {
        return data[row * data_cols + col];
    }
    std::vector<SCALAR> getData() const { return data; }

    int size() const { return data.size(); }

    void setElement(int index, SCALAR value) { data[index] = value; }

    void setElement(const int row, const int col, SCALAR value) {
        data[row * data_cols + col] = value;
    }

   private:
    std::vector<SCALAR> data;
    int data_cols = -1;
    int data_rows = -1;
    Options option;
};

#define MAKE_TYPEDEFS(Type, TypeSuffix, Size, SizeSuffix)            \
    typedef Matrix<Type, Size, Size> Matrix##SizeSuffix##TypeSuffix; \
    typedef Matrix<Type, Size, 1> Vector##SizeSuffix##TypeSuffix;    \
    typedef Matrix<Type, 1, Size> RowVector##SizeSuffix##TypeSuffix;

#define MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, Size)                  \
    typedef Matrix<Type, Size, Dynamic> Matrix##Size##X##TypeSuffix; \
    typedef Matrix<Type, Dynamic, Size> Matrix##X##Size##TypeSuffix;

#define MAKE_TYPEDEFS_ALL_SIZES(Type, TypeSuffix) \
    MAKE_TYPEDEFS(Type, TypeSuffix, 2, 2)         \
    MAKE_TYPEDEFS(Type, TypeSuffix, 3, 3)         \
    MAKE_TYPEDEFS(Type, TypeSuffix, 4, 4)         \
    MAKE_TYPEDEFS(Type, TypeSuffix, Dynamic, X)   \
    MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 2)      \
    MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 3)      \
    MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 4)
MAKE_TYPEDEFS_ALL_SIZES(int, i)
MAKE_TYPEDEFS_ALL_SIZES(float, f)
MAKE_TYPEDEFS_ALL_SIZES(double, d)

#undef MAKE_TYPEDEFS_ALL_SIZES
#undef MAKE_TYPEDEFS
#undef MAKE_FIXED_TYPEDEFS
}  // namespace internal
#endif
