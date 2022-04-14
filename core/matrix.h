#ifndef __CLRS4_MATRIX_H__
#define __CLRS4_MATRIX_H__

#include <core/common.h>
#include <initializer_list>
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

template <semiregular T, bool Const> class MatrixView;

template <semiregular T> class Matrix {
public:
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
    copy(cbegin(other.buffer_), cend(other.buffer_), begin(buffer_));
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

  [[nodiscard]] size_t rows() const { return rows_; }

  [[nodiscard]] size_t cols() const { return cols_; }

  MatrixView<T, false> submatrix(size_t r1, size_t c1) {
    return submatrix(r1, c1, rows(), cols());
  }

  MatrixView<T, false> submatrix(size_t r1, size_t c1, size_t r2, size_t c2) {
    return as_const(*this).submatrix(r1, c1, r2, c2);
  }

  MatrixView<T, true> submatrix(size_t r1, size_t c1) const {
    return submatrix(r1, c1, rows(), cols());
  }

  MatrixView<T, true> submatrix(size_t r1, size_t c1, size_t r2,
                                size_t c2) const {
    if (r1 >= r2 || c1 >= c2) {
      throw invalid_argument("submatrix dimension is invalid\n");
    }
    MatrixView<T, true> sub(r2 - r1, c2 - c1, &buffer_[r1 * cols_ + c1], rows_,
                            cols_);
    return sub;
  }

private:
  size_t rows_;
  size_t cols_;
  unique_ptr<T[]> buffer_;
};

template <semiregular T, bool Const> class MatrixView {
public:
  using pointer = conditional_t<Const, const T *, T *>;
  using reference = conditional_t<Const, const T &, T &>;

  MatrixView(size_t rows, size_t cols, pointer view, size_t orig_rows,
             size_t orig_cols)
      : rows_{rows}, cols_{cols}, view_{view}, orig_cols_{orig_cols} {
    if (rows > orig_rows || cols > orig_cols) {
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
      : rows_{other.rows()}, cols_{other.cols()}, orig_cols_{other.orig_cols_} {
    view_ = const_cast<pointer>(other.view());
  }

public:
  reference operator()(size_t r, size_t c) {
    assert(r < rows_ && c < cols_);
    return view_[r * cols_ + c];
  }

  const T &operator()(size_t r, size_t c) const {
    assert(r < rows_ && c < cols_);
    return view_[r * cols_ + c];
  }

  [[nodiscard]] size_t rows() const { return rows_; }

  [[nodiscard]] size_t cols() const { return cols_; }

  [[nodiscard]] pointer view() const { return view_; }

  friend class Matrix<T>;
  friend class MatrixView<T, !Const>;

private:
  size_t rows_;
  size_t cols_;
  size_t orig_cols_;
  pointer view_;
};

} // namespace frozenca

#endif //__CLRS4_MATRIX_H__
