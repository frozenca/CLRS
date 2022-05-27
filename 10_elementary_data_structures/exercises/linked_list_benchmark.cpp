#include <algorithm>
#include <chrono>
#include <iostream>
#include <linkedlist.h>
#include <list>
#include <random>
#include <ranges>
#include <test.h>
#include <util.h>
#include <vector>

namespace fc = frozenca;
using namespace std;

template <typename ListType> void linked_list_test() {
  constexpr int max_n = 5'000;
  constexpr int max_trials = 200;

  mt19937 gen(random_device{}());
  uniform_int_distribution<> dist(0, max_n);
  vector<float> durations_insert;
  vector<float> durations_erase;
  for (int t = 0; t < max_trials; ++t) {
    ListType l;

    float duration = 0.0f;
    for (int i = 0; i < max_n; ++i) {
      auto num = dist(gen);
      auto start = chrono::steady_clock::now();
      l.insert(ranges::lower_bound(l, num), num);
      if (!ranges::is_sorted(l)) {
        cout << "Verification fail!\n";
      }
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_insert.push_back(duration);

    duration = 0.0f;
    for (int i = 0; i < max_n; ++i) {
      auto num = dist(gen);
      auto start = chrono::steady_clock::now();
      auto it = ranges::find(l, num);
      if (it != ranges::end(l)) {
        l.erase(it);
        if (!ranges::is_sorted(l)) {
          cout << "Verification fail!\n";
        }
      }
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_erase.push_back(duration);
  }
  {
    auto stat = fc::get_statistics(durations_insert);
    fc::log("Time to insert {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_n, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }
  {
    auto stat = fc::get_statistics(durations_erase);
    fc::log("Time to erase {:6} elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, max_n, stat.average, stat.stdev,
            stat.percentile_95, stat.percentile_99, stat.percentile_999);
  }
}

int main() {
  cout << "Linked list demo\n";
  cout << "frozenca::hard::LinkedList<int> test\n";
  linked_list_test<fc::hard::LinkedList<int>>();
  cout << "std::list<int> test\n";
  linked_list_test<list<int>>();
}