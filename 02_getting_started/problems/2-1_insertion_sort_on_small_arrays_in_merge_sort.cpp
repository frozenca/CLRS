#include <02_getting_started/inc/easy.h>
#include <02_getting_started/inc/hard.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> v{2, 3, 1, 6, 5, 4};
    fc::hard::insertion_and_merge_sort(v);
    fc::print(v);
    fc::verify_sorting(ranges::sort);
    fc::verify_sorting(fc::hard::insertion_and_merge_sort);
    fc::perf_check_sorting(ranges::sort);
    fc::perf_check_sorting(fc::hard::insertion_and_merge_sort);
  }
}