#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<int> v{0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
  auto LIS = fc::longest_increasing_subsequence(v);
  for (auto n : LIS) {
    cout << n << ' ';
  }
}