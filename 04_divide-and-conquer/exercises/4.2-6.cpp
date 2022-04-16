#include <cassert>
#include <iostream>
#include <complex>

int main() {
  using namespace std;
  using namespace std::complex_literals;
  complex<int> z1 = 1. + 2i;
  complex<int> z2 = 3. + 4i;
  int a = z1.real();
  int b = z1.imag();
  int c = z2.real();
  int d = z2.imag();
  int ac = a * c;
  int bd = b * d;
  int a_bc_d = (a + b) * (c + d);
  complex<int> z3 = (ac - bd) * 1. + (a_bc_d - ac - bd) * 1. * 1i;
  if (z1 * z2 == z3) {
    cout << "complex multiplication succeeded\n";
  } else {
    cout << "complex multiplication failed\n";
  }
}