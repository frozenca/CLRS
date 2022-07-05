#include <14_dynamic_programming/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::index_t> B{2, 8, 10};
  cout << fc::break_string_lowest_cost(20, B);
}