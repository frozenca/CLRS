#include <06_heapsort/inc/easy.h>
#include <algorithm>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> A {27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::max_heapify(A, ssize(A) / 2 - 1, ssize(A));
    fc::print(A);
  }

  {
    vector<int> A {27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::max_heapify_iterative(A, ssize(A) / 2 - 1, ssize(A));
    fc::print(A);
  }
}