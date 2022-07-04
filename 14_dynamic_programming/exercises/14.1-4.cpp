#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::index_t> p{1, 5, 8, 9, 10, 17, 17, 20, 24, 30};

  cout << fc::cut_rod_memoized(p) << '\n';

  cout << fc::cut_rod_memoized_half(p) << '\n';

}