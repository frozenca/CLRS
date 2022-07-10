#include <16_amortized_analysis/inc/easy.h>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  auto codes = fc::get_gray_codes<3>();
  for (const auto &code : codes) {
    cout << code << ' ';
  }
  cout << '\n';

  constexpr size_t k = 22;
  auto t1 = chrono::steady_clock::now();
  auto codes_huge = fc::get_gray_codes<k>();
  auto t2 = chrono::steady_clock::now();
  auto d1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
  cout << d1 << "ms\n";
}
