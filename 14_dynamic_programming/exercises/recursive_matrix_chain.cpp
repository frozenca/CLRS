#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::index_t> p{30, 35, 15, 5, 10, 20, 25};
  cout << fc::matrix_chain_memoized(p);
}