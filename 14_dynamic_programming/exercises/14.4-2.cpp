#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<char> X{'A', 'B', 'C', 'B', 'D', 'A', 'B'};
  vector<char> Y{'B', 'D', 'C', 'A', 'B', 'A'};

  // both are correct
  fc::print_lcs(X, Y);
  cout << '\n';
  fc::print_lcs2(X, Y);
}