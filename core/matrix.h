#ifndef __CLRS4_MATRIX_H__
#define __CLRS4_MATRIX_H__

#include <common.h>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace frozenca {

using namespace std;
template <semiregular T>
using MatrixInitializer = initializer_list<initializer_list<T>>;

template <semiregular T>
bool check_non_jagged(const MatrixInitializer<T> &init) {
  auto i = cbegin(init);
  for (auto j = next(i); j != cend(init); ++j) {
    if (i->size() != j->size()) {
      return false;
    }
  }
  return true;
}

template <semiregular T>
void insert_flat(unique_ptr<T[]> &data, const MatrixInitializer<T> &init) {
  index_t index = 0;
  auto curr_row = cbegin(init);
  auto last_row = cend(init);
  for (; curr_row != last_row; ++curr_row) {
    auto curr = cbegin(*curr_row);
    auto last = cend(*curr_row);
    for (; curr != last; ++curr) {
      data[index] = *curr;
      ++index;
    }
  }
}

template <semiregular T> class Matrix;

template <semiregular T, bool Const = false> class MatrixView;

template <typename Mat>
concept MatType = requires(Mat m, size_t r, size_t c) {
  m(r, c);
  m.rows();
  m.cols();
  m.submatrix(r, c);
};

template <semiregular T> class Matrix {
public:
  using value_type = T;
  Matrix(size_t rows, size_t cols)
      : rows_{rows}, cols_{cols}, buffer_(make_unique<T[]>(rows_ * cols_)) {
    if (!rows_ || !cols_) {
      throw invalid_argument("Number of rows or cols are zero\n");
    }
  }

  explicit Matrix(MatrixInitializer<T> init)
      : rows_{size(init)}, cols_{size(*cbegin(init))},
        buffer_(make_unique<T[]>(rows_ * cols_)) {
    if (!rows_ || !cols_) {
      throw invalid_argument("Number of rows or cols are zero\n");
    }
    if (!check_non_jagged(init)) {
      throw invalid_argument("Jagged matrix initializer\n");
    }
    insert_flat(buffer_, init);
  }

  friend void swap(Matrix &a, Matrix &b) noexcept {
    swap(a.rows_, b.rows_);
    swap(a.cols_, b.cols_);
    swap(a.buffer_, b.buffer_);
  }

  Matrix(const Matrix &other)
      : rows_{other.rows_}, cols_{other.cols_},
        buffer_(make_unique<T[]>(rows_ * cols_)) {
    copy(other.buffer_.get(), other.buffer_.get() + rows_ * cols_,
         buffer_.get());
  }

  Matrix &operator=(const Matrix &other) {
    auto tmp = other;
    swap(*this, tmp);
    return *this;
  }

  Matrix(Matrix &&other) = default;
  Matrix &operator=(Matrix &&other) = default;

  T &operator()(size_t r, size_t c) {
    assert(r < rows_ && c < cols_);
    return buffer_[r * cols_ + c];
  }

  const T &operator()(size_t r, size_t c) const {
    assert(r < rows_ && c < cols_);
    return buffer_[r * cols_ + c];
  }

  void zero_fill() { fill(buffer_.get(), buffer_.get() + rows_ * cols_, T{}); }

  void random_fill() requires is_integral_v<T> {
    mt19937 gen(random_device{}());
    uniform_int_distribution<T> dist(0, 500);
    generate(buffer_.get(), buffer_.get() + rows_ * cols_,
             [&dist, &gen]() { return dist(gen); });
  }

  void random_fill() requires is_floating_point_v<T> {
    mt19937 gen(random_device{}());
    uniform_real_distribution<T> dist(T{-300}, T{300});
    generate(buffer_.get(), buffer_.get() + rows_ * cols_,
             [&dist, &gen]() { return dist(gen); });
  }

  [[nodiscard]] size_t urows() const { return rows_; }

  [[nodiscard]] size_t ucols() const { return cols_; }

  [[nodiscard]] index_t rows() const { return static_cast<index_t>(rows_); }

  [[nodiscard]] index_t cols() const { return static_cast<index_t>(cols_); }

  MatrixView<T> submatrix(size_t r1, size_t c1) {
    return submatrix(r1, c1, urows(), ucols());
  }

  MatrixView<T> submatrix(size_t r1, size_t c1, size_t r2, size_t c2) {
    return as_const(*this).submatrix(r1, c1, r2, c2);
  }

  MatrixView<T, true> submatrix(size_t r1, size_t c1) const {
    return submatrix(r1, c1, urows(), ucols());
  }

  MatrixView<T, true> submatrix(size_t r1, size_t c1, size_t r2,
                                size_t c2) const {
    if (r1 >= r2 || c1 >= c2 || r2 > rows_ || c2 > cols_) {
      throw invalid_argument("submatrix dimension is invalid\n");
    }
    MatrixView<T, true> sub(r2 - r1, c2 - c1, &buffer_[r1 * cols_ + c1], cols_);
    return sub;
  }

  template <MatType Mat>
  Matrix(const Mat &other) : Matrix(other.urows(), other.ucols()) {
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        (*this)(r, c) = other(r, c);
      }
    }
  }

  template <MatType Mat> Matrix &operator+=(const Mat &other) {
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        (*this)(r, c) += other(r, c);
      }
    }
    return *this;
  }

  template <MatType Mat> Matrix &operator-=(const Mat &other) {
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        (*this)(r, c) += other(r, c);
      }
    }
    return *this;
  }

  friend class MatrixView<T, false>;
  friend class MatrixView<T, true>;

private:
  size_t rows_;
  size_t cols_;
  unique_ptr<T[]> buffer_;
};

template <semiregular T, bool Const> class MatrixView {
public:
  using value_type = T;
  using pointer = conditional_t<Const, const T *, T *>;
  using reference = conditional_t<Const, const T &, T &>;

  MatrixView(size_t rows, size_t cols, pointer view, size_t orig_cols)
      : rows_{rows}, cols_{cols}, view_{view}, orig_cols_{orig_cols} {
    if (cols > orig_cols) {
      throw invalid_argument("Dimension of matrix view exceeds dimension of "
                             "the original matrix\n");
    }
    if (!rows_ || !cols_) {
      throw invalid_argument("Number of rows or cols are zero\n");
    }
  }

private:
  template <bool ConstOther>
  MatrixView(const MatrixView<T, ConstOther> &other)
      : rows_{other.urows()}, cols_{other.ucols()}, orig_cols_{
                                                        other.orig_cols_} {
    view_ = const_cast<pointer>(other.view());
  }

public:
  reference operator()(size_t r, size_t c) {
    assert(r < rows_ && c < cols_);
    return view_[r * orig_cols_ + c];
  }

  const T &operator()(size_t r, size_t c) const {
    assert(r < rows_ && c < cols_);
    return view_[r * orig_cols_ + c];
  }

  [[nodiscard]] size_t urows() const { return rows_; }

  [[nodiscard]] size_t ucols() const { return cols_; }

  [[nodiscard]] index_t rows() const { return static_cast<index_t>(rows_); }

  [[nodiscard]] index_t cols() const { return static_cast<index_t>(cols_); }

  [[nodiscard]] pointer view() const { return view_; }

  MatrixView<T, Const> submatrix(size_t r1, size_t c1) {
    return submatrix(r1, c1, urows(), ucols());
  }

  MatrixView<T, Const> submatrix(size_t r1, size_t c1, size_t r2, size_t c2) {
    return as_const(*this).submatrix(r1, c1, r2, c2);
  }

  MatrixView<T, true> submatrix(size_t r1, size_t c1) const {
    return submatrix(r1, c1, urows(), ucols());
  }

  MatrixView<T, true> submatrix(size_t r1, size_t c1, size_t r2,
                                size_t c2) const {
    if (r1 >= r2 || c1 >= c2) {
      throw invalid_argument("submatrix dimension is invalid\n");
    }
    MatrixView<T, true> sub(r2 - r1, c2 - c1, &view_[r1 * orig_cols_ + c1],
                            orig_cols_);
    return sub;
  }

  friend class Matrix<T>;
  friend class MatrixView<T, !Const>;

  template <MatType Mat>
  MatrixView &operator+=(const Mat &mat) requires(!Const) {
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        (*this)(r, c) += mat(r, c);
      }
    }
    return *this;
  }

  template <MatType Mat>
  MatrixView &operator-=(const Mat &mat) requires(!Const) {
    for (size_t r = 0; r < rows_; ++r) {
      for (size_t c = 0; c < cols_; ++c) {
        (*this)(r, c) -= mat(r, c);
      }
    }
    return *this;
  }

private:
  size_t rows_;
  size_t cols_;
  size_t orig_cols_;
  pointer view_;
};

template <MatType Mat1, MatType Mat2>
auto operator+(const Mat1 &A, const Mat2 &B) {
  if (A.rows() != B.rows() || A.cols() != B.cols()) {
    throw invalid_argument("Matrix dimensions do not match\n");
  }
  index_t rows = A.rows();
  index_t cols = A.cols();
  using T = common_type_t<typename Mat1::value_type, typename Mat2::value_type>;  
  Matrix<T> C(rows, cols);
  for (index_t r = 0; r < rows; ++r) {
    for (index_t c = 0; c < cols; ++c) {
      C(r, c) = A(r, c) + B(r, c);
    }
  }
  return C;
}

template <MatType Mat1, MatType Mat2>
auto operator-(const Mat1 &A, const Mat2 &B) {
  if (A.rows() != B.rows() || A.cols() != B.cols()) {
    throw invalid_argument("Matrix dimensions do not match\n");
  }
  index_t rows = A.rows();
  index_t cols = A.cols();
  using T = common_type_t<typename Mat1::value_type, typename Mat2::value_type>;  
  Matrix<T> C(rows, cols);
  for (index_t r = 0; r < rows; ++r) {
    for (index_t c = 0; c < cols; ++c) {
      C(r, c) = A(r, c) - B(r, c);
    }
  }
  return C;
}

template <MatType Mat1, MatType Mat2>
auto operator*(const Mat1 &A, const Mat2 &B) {
  if (A.cols() != B.rows()) {
    throw invalid_argument("Matrix dimensions do not match\n");
  }
  index_t rows = A.rows();
  index_t cols = B.cols();
  using T = common_type_t<typename Mat1::value_type, typename Mat2::value_type>;  
  Matrix<T> C(rows, cols);
  C.zero_fill();
  for (index_t r = 0; r < rows; ++r) {
    for (index_t c = 0; c < cols; ++c) {
      for (index_t k = 0; k < A.cols(); ++k) {
        C(r, c) += A(r, k) * B(k, c);
      }
    }
  }
  return C;
}

template <MatType Mat> ostream &operator<<(ostream &os, const Mat &m) {
  os << '{';
  for (index_t r = 0; r < m.rows(); ++r) {
    os << '{';
    for (index_t c = 0; c < m.cols(); ++c) {
      os << m(r, c) << ((c < m.cols() - 1) ? ", " : "");
    }
    os << '}' << ((r < m.rows() - 1) ? ",\n" : "");
  }
  os << '}';
  return os;
}

} // namespace frozenca

#endif //__CLRS4_MATRIX_H__
