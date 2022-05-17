#include <08_sorting_in_linear_time/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <random>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A{2, 5, 3, 0, 2, 3, 0, 3};
    auto B = fc::counting_range_query(A, 5, {{2, 5}, {2, 3}});
    for (auto count : B) {
      cout << count << ' ';
    }
    cout << '\n';
  }

}