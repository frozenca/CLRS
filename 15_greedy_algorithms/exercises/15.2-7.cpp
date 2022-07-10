#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::index_t> A{1, 2, 3, 5, 6};
  vector<fc::index_t> B{6, 5, 3, 2, 1};
  cout << fc::maximize_payoff(A, B);
}
