#ifndef __CLRS4_MATRIX_MULTIPLY_H__
#define __CLRS4_MATRIX_MULTIPLY_H__

#include <matrix.h>

namespace frozenca {

using namespace std;

namespace {

template <semiregular T>
void matrix_multiply_impl(const Matrix<T> &A, const Matrix<T> &B,
                          Matrix<T> &C) {
  index_t m = A.rows();
  index_t p = A.cols();
  index_t n = B.cols();
  for (index_t i = 0; i < m; ++i) {
    for (index_t j = 0; j < n; ++j) {
      for (index_t k = 0; k < p; ++k) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

template <semiregular T>
void matrix_multiply_impl(const MatrixView<T, true> &A,
                          const MatrixView<T, true> &B,
                          MatrixView<T, false> &C) {
  index_t m = A.rows();
  index_t p = A.cols();
  index_t n = B.cols();
  for (index_t i = 0; i < m; ++i) {
    for (index_t j = 0; j < n; ++j) {
      for (index_t k = 0; k < p; ++k) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

template <semiregular T>
void matrix_multiply_recursive_impl(const MatrixView<T, true> &A,
                                    const MatrixView<T, true> &B,
                                    MatrixView<T, false> &C) {
  index_t m = A.rows();
  index_t p = A.cols();
  index_t n = B.cols();
  if (m == 1 || n == 1 || p == 1) {
    matrix_multiply_impl(A, B, C);
    return;
  }
  auto m1 = m / 2;
  auto p1 = p / 2;
  auto n1 = n / 2;
  auto A11 = A.submatrix(0, 0, m1, p1);
  auto A12 = A.submatrix(0, p1, m1, p);
  auto A21 = A.submatrix(m1, 0, m, p1);
  auto A22 = A.submatrix(m1, p1, m, p);
  auto B11 = B.submatrix(0, 0, p1, n1);
  auto B12 = B.submatrix(0, n1, p1, n);
  auto B21 = B.submatrix(p1, 0, p, n1);
  auto B22 = B.submatrix(p1, n1, p, n);
  auto C11 = C.submatrix(0, 0, m1, n1);
  auto C12 = C.submatrix(0, n1, m1, n);
  auto C21 = C.submatrix(m1, 0, m, n1);
  auto C22 = C.submatrix(m1, n1, m, n);
  matrix_multiply_recursive_impl(A11, B11, C11);
  matrix_multiply_recursive_impl(A11, B12, C12);
  matrix_multiply_recursive_impl(A21, B11, C21);
  matrix_multiply_recursive_impl(A21, B12, C22);
  matrix_multiply_recursive_impl(A12, B21, C11);
  matrix_multiply_recursive_impl(A12, B22, C12);
  matrix_multiply_recursive_impl(A22, B21, C21);
  matrix_multiply_recursive_impl(A22, B22, C22);
}

} // anonymous namespace

template <semiregular T>
Matrix<T> matrix_multiply(const Matrix<T> &A, const Matrix<T> &B) {
  index_t m = A.rows();
  index_t p = A.cols();
  index_t n = B.cols();
  if (p != B.rows()) {
    throw invalid_argument(
        "In matrix multiply dimension of two matrices don't match\n");
  }
  Matrix<T> C(m, n);
  C.zero_fill();
  matrix_multiply_impl(A, B, C);
  return C;
}

template <semiregular T>
Matrix<T> matrix_multiply_recursive(const Matrix<T> &A, const Matrix<T> &B) {
  index_t m = A.rows();
  index_t p = A.cols();
  index_t n = B.cols();
  if (p != B.rows()) {
    throw invalid_argument(
        "In matrix multiply dimension of two matrices don't match\n");
  }
  Matrix<T> C(m, n);
  C.zero_fill();
  auto A_view = A.submatrix(0, 0, m, p);
  auto B_view = B.submatrix(0, 0, p, n);
  auto C_view = C.submatrix(0, 0, m, n);
  matrix_multiply_recursive_impl(A_view, B_view, C_view);
  return C;
}

} // namespace frozenca

#endif //__CLRS4_MATRIX_MULTIPLY_H__
