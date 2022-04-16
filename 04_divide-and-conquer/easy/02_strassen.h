#ifndef __CLRS4_STRASSEN_H__
#define __CLRS4_STRASSEN_H__

#include <bit>
#include <matrix.h>

namespace frozenca {

using namespace std;

template <MatType Mat1, MatType Mat2>
auto strassen(const Mat1 &A, const Mat2 &B) {
  auto N = A.urows();
  if (A.ucols() != N || B.urows() != N || B.ucols() != N ||
      !has_single_bit(N)) {
    throw invalid_argument(
        "Can't use Strassen's algorithm, dimension is invalid\n");
  }
  if (N == 1u) {
    return A * B;
  }
  using T = common_type_t<typename Mat1::value_type, typename Mat2::value_type>;
  Matrix<T> C(N, N);
  C.zero_fill();
  auto H = N / 2;
  auto A11 = A.submatrix(0, 0, H, H);
  auto A12 = A.submatrix(0, H, H, N);
  auto A21 = A.submatrix(H, 0, N, H);
  auto A22 = A.submatrix(H, H, N, N);
  auto B11 = B.submatrix(0, 0, H, H);
  auto B12 = B.submatrix(0, H, H, N);
  auto B21 = B.submatrix(H, 0, N, H);
  auto B22 = B.submatrix(H, H, N, N);
  auto C11 = C.submatrix(0, 0, H, H);
  auto C12 = C.submatrix(0, H, H, N);
  auto C21 = C.submatrix(H, 0, N, H);
  auto C22 = C.submatrix(H, H, N, N);
  auto P1 = strassen(A11, B12 - B22);
  auto P2 = strassen(A11 + A12, B22);
  auto P3 = strassen(A21 + A22, B11);
  auto P4 = strassen(A22, B21 - B11);
  auto P5 = strassen(A11 + A22, B11 + B22);
  auto P6 = strassen(A12 - A22, B21 + B22);
  auto P7 = strassen(A11 - A21, B11 + B12);

  C11 += P5;
  C11 += P4;
  C11 -= P2;
  C11 += P6;
  
  C12 += P1;
  C12 += P2;
  
  C21 += P3;
  C21 += P4;

  C22 += P5;
  C22 += P1;
  C22 -= P3;
  C22 -= P7;
  
  return C;
}

} // namespace frozenca

#endif //__CLRS4_MATRIX_MULTIPLY_H__
