#include <16_amortized_analysis/inc/easy.h>
#include <cassert>
#include <chrono>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  constexpr int nums = 1'000'000;

  mt19937 gen(random_device{}());
  uniform_int_distribution<> dist(1, 2);
  uniform_int_distribution<> num_dist(1, 100);
  vector<int> v;
  auto t1 = chrono::steady_clock::now();
  for (int i = 0; i < nums; i++) {
    switch (dist(gen)) {
    case 1:
      v.push_back(num_dist(gen));
      break;
    case 2:
      fc::delete_larger_half(v);
      break;
    default:
      assert(false);
      break;
    }
  }
  auto t2 = chrono::steady_clock::now();
  auto d1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
  cout << d1 << "ms\n";
}
