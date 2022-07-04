#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<char> X{'A', 'B', 'C', 'B', 'D', 'A', 'B'};
  vector<char> Y{'B', 'D', 'C', 'A', 'B', 'A'};

  fc::print_lcs(X, Y);
  cout << '\n';
  cout << fc::lcs_length_memoized(X, Y);
}