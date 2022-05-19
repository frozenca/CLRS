#include <10_elementary_data_structures/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    fc::CompactList<int> l;
    l.list_insert(7);
    cout << l << '\n';
    l.list_insert(3);
    cout << l << '\n';
    l.list_insert(6);
    cout << l << '\n';
    l.list_insert(2);
    cout << l << '\n';
    l.list_insert(5);
    cout << l << '\n';
    l.list_delete(6);
    cout << l << '\n';
    l.list_delete(3);
    cout << l << '\n';
    l.list_delete(2);
    cout << l << '\n';
    l.list_delete(7);
    cout << l << '\n';
    l.list_delete(5);
    cout << l << '\n';
  }

  constexpr int num_trials = 10'000;
  constexpr fc::uindex_t N = 10'000;
  {

    vector<float> durations;
    fc::CompactList<int, N> l;
    vector<int> v(N);
    mt19937 gen(random_device{}());
    iota(v.begin(), v.end(), 0);
    ranges::shuffle(v, gen);
    for (auto n : v) {
      l.list_insert(n);
    }
    uniform_int_distribution<> num_dist(0, N - 1);
    for (int i = 0; i < num_trials; ++i) {
      auto num = num_dist(gen);
      auto start = chrono::steady_clock::now();
      l.list_search_compact(num);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("compact list search Time to process a range of {:6} "
            "elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, N, stat.average, stat.stdev, stat.percentile_95,
            stat.percentile_99, stat.percentile_999);
  }

  {

    vector<float> durations;
    fc::CompactList<int, N> l;
    vector<int> v(N);
    mt19937 gen(random_device{}());
    iota(v.begin(), v.end(), 0);
    ranges::shuffle(v, gen);
    for (auto n : v) {
      l.list_insert(n);
    }
    uniform_int_distribution<> num_dist(0, N - 1);
    for (int i = 0; i < num_trials; ++i) {
      auto num = num_dist(gen);
      auto start = chrono::steady_clock::now();
      l.list_search_compact2(num);
      auto end = chrono::steady_clock::now();
      durations.push_back(
          chrono::duration_cast<chrono::duration<float, micro>>(end - start)
              .count());
    }
    auto stat = fc::get_statistics(durations);
    fc::log("compact list search2 Time to process a range of {:6} "
            "elements:\n"
            "Average : {:10.4f} us,\n"
            "Stdev   : {:10.4f} us,\n"
            "95%     : {:10.4f} us,\n"
            "99%     : {:10.4f} us,\n"
            "99.9%   : {:10.4f} us,\n",
            fc::log_level::all, N, stat.average, stat.stdev, stat.percentile_95,
            stat.percentile_99, stat.percentile_999);
  }
}