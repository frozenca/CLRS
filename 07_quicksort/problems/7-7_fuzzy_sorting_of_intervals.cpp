#include <07_quicksort/inc/easy.h>
#include <07_quicksort/inc/hard.h>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <util.h>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::Interval> v;
  constexpr int num_intervals = 5'000;
  mt19937 gen(random_device{}());
  uniform_real_distribution<float> dist(0.0f, 10.0f);
  for (int i = 0; i < num_intervals; i++) {
    auto a = dist(gen);
    auto b = dist(gen);
    if (a <= b) {
      v.emplace_back(a, b);
    } else {
      v.emplace_back(b, a);
    }
  }
  auto start = chrono::steady_clock::now();
  fc::fuzzy_sort(v);
  auto end = chrono::steady_clock::now();
  float c = 0.0f;
  for (const auto &itv : v) {
    c = max(c, itv.first());
    if (c > itv.second()) {
      throw runtime_error("the algorithm is wrong!");
    }
  }
  auto elapsed =
      chrono::duration_cast<chrono::duration<float, milli>>(end - start)
          .count();
  fc::log("Successfully sorted {} intervals.\n"
          "Elapsed time: {:10.4f} ms\n",
          fc::log_level::all, num_intervals, elapsed);
}