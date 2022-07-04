#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::index_t> p{0, 4, 7, 8};

  cout << fc::cut_rod_memoized(p) << '\n';

  cout << fc::cut_rod_greedy(p) << '\n';
}