#include <06_heapsort/inc/easy.h>
#include <algorithm>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::heapsort(A);
    fc::print(A);
  }

}