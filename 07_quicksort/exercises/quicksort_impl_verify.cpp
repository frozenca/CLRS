#include <07_quicksort/inc/easy.h>
#include <07_quicksort/inc/hard.h>
#include <algorithm>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  
  {
    vector<int> A{27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0};
    fc::hard::quicksort(A);
    fc::print(A);
    if (ranges::is_sorted(A)) {
      cout << "Sorted\n";
    } else {
      cout << "Failed to sort\n";
    }
  }
  fc::verify_sorting(fc::hard::quicksort);
  fc::verify_sorting(fc::hard::randomized_quicksort);
  fc::perf_check_sorting(ranges::sort);
  fc::perf_check_sorting(fc::hard::quicksort);
  fc::perf_check_sorting(fc::hard::randomized_quicksort); // much much slower!
  
}