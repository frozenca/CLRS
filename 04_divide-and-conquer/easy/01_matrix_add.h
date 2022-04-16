#ifndef __CLRS4_MATRIX_ADD_H__
#define __CLRS4_MATRIX_ADD_H__

#include <matrix.h>

namespace frozenca {

using namespace std;

namespace {

template <semiregular T>
void matrix_add_impl(const MatrixView<T, true> &A, const MatrixView<T, true> &B,
                     MatrixView<T, false> &C) {
  index_t m = A.rows();
  index_t n = A.cols();
  for (index_t i = 0; i < m; ++i) {
    for (index_t j = 0; j < n; ++j) {
      C(i, j) = A(i, j) + B(i, j);
    }
  }
}

template <semiregular T>
void matrix_add_recursive_impl(const MatrixView<T, true> &A, const MatrixView<T, true> &B,
                     MatrixView<T, false> &C) {
  index_t m = A.rows();
  index_t n = A.cols();
  if (m == 1 || n == 1) {
    matrix_add_impl(A, B, C);
    return;
  }
  auto m1 = m / 2;
  auto n1 = n / 2;
  auto A11 = A.submatrix(0, 0, m1, n1);
  auto A12 = A.submatrix(0, n1, m1, n);
  auto A21 = A.submatrix(m1, 0, m, n1);
  auto A22 = A.submatrix(m1, n1, m, n);
  auto B11 = B.submatrix(0, 0, m1, n1);
  auto B12 = B.submatrix(0, n1, m1, n);
  auto B21 = B.submatrix(m1, 0, m, n1);
  auto B22 = B.submatrix(m1, n1, m, n);
  auto C11 = C.submatrix(0, 0, m1, n1);
  auto C12 = C.submatrix(0, n1, m1, n);
  auto C21 = C.submatrix(m1, 0, m, n1);
  auto C22 = C.submatrix(m1, n1, m, n);
  matrix_add_recursive_impl(A11, B11, C11);
  matrix_add_recursive_impl(A12, B12, C12);
  matrix_add_recursive_impl(A21, B21, C21);
  matrix_add_recursive_impl(A22, B22, C22);
}

} // anonymous namespace

template <semiregular T>
Matrix<T> matrix_add_recursive(const Matrix<T> &A, const Matrix<T> &B) {
  index_t m = A.rows();
  index_t n = A.cols();
  if (A.rows() != B.rows() || A.cols() != B.cols()) {
    throw invalid_argument("Matrix dimensions do not match\n");
  }
  Matrix<T> C(m, n);
  C.zero_fill();
  auto A_view = A.submatrix(0, 0, m, n);
  auto B_view = B.submatrix(0, 0, m, n);
  auto C_view = C.submatrix(0, 0, m, n);
  matrix_add_recursive_impl(A_view, B_view, C_view);
  return C;
}


template <semiregular T>
Matrix<T> matrix_add_recursive_copy(const Matrix<T> &A, const Matrix<T> &B) {
  index_t m = A.rows();
  index_t n = A.cols();
  if (A.rows() != B.rows() || A.cols() != B.cols()) {
    throw invalid_argument("Matrix dimensions do not match\n");
  }
  if (m == 1 || n == 1) {
    return A + B;
  }
  Matrix<T> C(m, n);
  C.zero_fill();
  auto m1 = m / 2;
  auto n1 = n / 2;
  Matrix<T> A11 (A.submatrix(0, 0, m1, n1));
  Matrix<T> A12 (A.submatrix(0, n1, m1, n));
  Matrix<T> A21 (A.submatrix(m1, 0, m, n1));
  Matrix<T> A22 (A.submatrix(m1, n1, m, n));
  Matrix<T> B11 (B.submatrix(0, 0, m1, n1));
  Matrix<T> B12 (B.submatrix(0, n1, m1, n));
  Matrix<T> B21 (B.submatrix(m1, 0, m, n1));
  Matrix<T> B22 (B.submatrix(m1, n1, m, n));
  auto C11 = C.submatrix(0, 0, m1, n1);
  auto C12 = C.submatrix(0, n1, m1, n);
  auto C21 = C.submatrix(m1, 0, m, n1);
  auto C22 = C.submatrix(m1, n1, m, n);
  C11 += matrix_add_recursive_copy(A11, B11);
  C12 += matrix_add_recursive_copy(A12, B12);
  C21 += matrix_add_recursive_copy(A21, B21);
  C22 += matrix_add_recursive_copy(A22, B22);
  return C;
}

} // namespace frozenca

#endif //__CLRS4_MATRIX_ADD_H__
