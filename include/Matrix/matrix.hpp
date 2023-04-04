#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <memory>
namespace BaseType {
  template <typename T>
  class Matrix {
    private:
      int rows_;
      int cols_;
      int elements_size;
      std::shared_ptr<T[]> data_;

    public:
      Matrix() = default;
      Matrix(int rows, int cols);
      Matrix(int rows, int cols, const T *data);
      Matrix(const Matrix<T>& other_matrix);
      ~Matrix();

    public:
      T get_element(int row, int col);
      T *data();
      const T* data() const;
      int get_rows() const;
      int get_cols() const;
      int get_element_size() const;

    public:
      bool operator==(const Matrix<T> other_matrix);

      // operations between matrixs
      template<typename U> friend Matrix<U> operator+ (const Matrix<U> left_matrix,const Matrix<U> right_matrix);
      template<typename U> friend Matrix<U> operator- (const Matrix<U> left_matrix,const Matrix<U> right_matrix);
      template<typename U> friend Matrix<U> operator* (const Matrix<U> left_matrix,const Matrix<U> right_matrix);

      // scalar operations
      template<typename U> friend Matrix<U> operator+ (const Matrix<U> left_matrix,const U& right_scalar);
      template<typename U> friend Matrix<U> operator- (const Matrix<U> left_matrix,const U& right_scalar);
      template<typename U> friend Matrix<U> operator* (const Matrix<U> left_matrix,const U& right_scalar);

      template<typename U> friend Matrix<U> operator+ (const U& left_scalar,const Matrix<U> right_matrix);
      template<typename U> friend Matrix<U> operator- (const U& left_scalar,const Matrix<U> right_matrix);
      template<typename U> friend Matrix<U> operator* (const U& left_scalar,const Matrix<U> right_matrix);
  };

  // template <typename T>
  // Matrix<T>::Matrix()
  //     :rows_(0),cols_(0),elements_size(0){
  //   data_.reset();
  // };

  template <typename T>
  Matrix<T>::Matrix(int rows, int cols)
      : rows_(rows), cols_(cols), elements_size(rows * cols) {
    data_=std::make_shared<T>(new T[elements_size]);
    // data_.reset(new T[elements_size]);
  };

  template <typename T>
  Matrix<T>::Matrix(int rows, int cols, const T *data)
      : Matrix<T>(rows, cols){
    T* dst = this->data();
    std::copy(data, data+this->get_element_size(), dst);
  };

  template <class T>
  Matrix<T>::~Matrix() {
    if (data_ != nullptr) {
      delete[] data_;
    }
  }

  template <class T>
  T* Matrix<T>::data() {
    return data_.get();
  }

  template <class T>
  const T* Matrix<T>::data() const{
    return data_.get();
  }
  template <class T> int Matrix<T>::get_rows() const { return rows_; }
  template <class T> int Matrix<T>::get_cols() const { return cols_; }
  template <class T> int Matrix<T>::get_element_size() const { return elements_size; }
  

} // namespace BaseType
#endif