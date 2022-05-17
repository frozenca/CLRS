#include <09_medians_and_order_statistics/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> v{4, 2, 1, 5, 3, 6};
    cout << fc::randomized_select(v, ssize(v) / 2) << '\n';
  }
  {
    vector<int> v{4, 2, 1, 5, 3, 6};
    cout << fc::randomized_select_iterative(v, ssize(v) / 2) << '\n';
  }
}