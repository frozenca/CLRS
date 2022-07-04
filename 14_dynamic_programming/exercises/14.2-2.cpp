#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::Matrix<int>> A(6);
  A[0] = fc::Matrix<int>(6, 7);
  A[1] = fc::Matrix<int>(7, 3);
  A[2] = fc::Matrix<int>(3, 1);
  A[3] = fc::Matrix<int>(1, 2);
  A[4] = fc::Matrix<int>(2, 4);
  A[5] = fc::Matrix<int>(4, 5);
  for (auto &m : A) {
    m.iota_fill();
  }
  auto M = fc::matrix_chain_multiply(A);
  cout << M << '\n';
}