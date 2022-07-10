#include <16_amortized_analysis/inc/easy.h>
#include <cassert>
#include <chrono>
#include <random>
#include <stack>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  constexpr int nums = 1'000'000;
  stack<int> s;

  auto t1 = chrono::steady_clock::now();
  for (int i = 0; i < nums / 2; ++i) {
    s.push(1);
  }
  for (int i = 0; i < nums / 2; ++i) {
    s.pop();
  }
  auto t2 = chrono::steady_clock::now();
  auto d1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
  cout << d1 << "ms\n";

  mt19937 gen(random_device{}());
  uniform_int_distribution<> dist(1, 3);
  t1 = chrono::steady_clock::now();
  for (int i = 0; i < nums; i++) {
    switch (dist(gen)) {
    case 1:
      s.push(1);
      break;
    case 2:
      if (!s.empty()) {
        s.pop();
      }
      break;
    case 3:
      fc::multipop(s, 30);
      break;
    default:
      assert(false);
      break;
    }
  }
  t2 = chrono::steady_clock::now();
  d1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
  cout << d1 << "ms\n";

  vector<int> v(nums);
  t1 = chrono::steady_clock::now();
  for (int i = 0; i < nums; ++i) {
    fc::increment(v);
  }
  t2 = chrono::steady_clock::now();
  d1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
  cout << d1 << "ms\n";
}
