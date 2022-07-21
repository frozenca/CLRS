#include <algorithm>
#include <btreedisk.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <ranges>
#include <set>
#include <test.h>
#include <util.h>
#include <vector>

namespace fc = frozenca;
using namespace std;

template <typename RBTreeType, typename Alloc>
void rb_tree_test(const Alloc &alloc, bool warmup = false) {
  constexpr int max_n = 1'000'000;
  constexpr int max_trials = 5;

  mt19937 gen(random_device{}());
  vector<float> durations_insert;
  vector<float> durations_find;
  vector<float> durations_erase;
  vector<int> v(max_n);
  iota(v.begin(), v.end(), 0);

  for (int t = 0; t < max_trials; ++t) {
    RBTreeType h(alloc);

    float duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      h.insert(num);
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_insert.push_back(duration);

    duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      if (!h.contains(num)) {
        cout << "Lookup verification fail!\n";
      }
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_find.push_back(duration);

    duration = 0.0f;
    ranges::shuffle(v, gen);
    for (auto num : v) {
      auto start = chrono::steady_clock::now();
      if (!h.erase(num)) {
        cout << "Erase verification fail!\n";
      }
      auto end = chrono::steady_clock::now();
      duration +=
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count();
    }
    durations_erase.push_back(duration);
  }
  if (!warmup) {
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
      auto stat = fc::get_statistics(durations_find);
      fc::log("Time to find {:6} elements:\n"
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
}

int main() {
  cout << "Red-black tree demo\n";

  fc::MemoryMappedFile mm_file("database.bin", 1UL << 31UL);
  fc::MemoryResourceFixed<int, 255> mem_res(
      reinterpret_cast<unsigned char *>(mm_file.data()), mm_file.size());
  fc::AllocatorFixed<int, 255> alloc_fixed(&mem_res);

  allocator<int> def_alloc;

  rb_tree_test<set<int>, allocator<int>>(def_alloc,
                                         true); // warm up for benchmarking

  cout << "Warming up complete...\n";
  cout << "frozenca::BTreeSet<int> test\n";
  rb_tree_test<fc::DiskBTree<int>, fc::AllocatorFixed<int, 255>>(alloc_fixed);
  cout << "std::set<int> test\n";
  rb_tree_test<set<int>, allocator<int>>(def_alloc);
}
