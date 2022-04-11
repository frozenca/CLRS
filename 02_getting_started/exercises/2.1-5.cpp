#include "02_getting_started/inc/easy.h"
#include "02_getting_started/inc/hard.h"
#include <bitset>
#include <iostream>


int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    size_t a = 37;
    size_t b = 26;
    bitset<8> A(a);
    bitset<8> B(b);
    auto C = fc::add_binary_integers(A, B);
    cout << C.to_ullong() << ' ' << a + b << '\n';
  }
}