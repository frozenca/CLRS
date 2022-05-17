#include <09_medians_and_order_statistics/inc/easy.h>
#include <common.h>
#include <iostream>
#include <test.h>
#include <util.h>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());

  constexpr int num_trials = 1'000;
  constexpr int max_n = 1'000;
  uniform_int_distribution<> dist(1, max_n);
  int n = dist(gen);
  uniform_int_distribution<> k_dist(2, n / 2);
  int k = k_dist(gen);

  int fail_count = 0;
  vector<float> durations;
  for (int i = 0; i < num_trials; ++i) {
    vector<int> v(n);
    iota(v.begin(), v.end(), 0);
    ranges::shuffle(v, gen);
    auto start = chrono::steady_clock::now();
    auto quantiles = fc::kth_quantiles(v, k);
    auto end = chrono::steady_clock::now();
    durations.push_back(
        chrono::duration_cast<chrono::duration<float, micro>>(end - start)
            .count());
    // verify
    int local_fail_count = 0;
    for (fc::index_t q = 0; q < k; ++q) {
      if (quantiles[q] != (n * q) / k) {
        cout << "Quantiles algorithm failed! " << quantiles[q] << ' '
             << (n * q) / k << '\n';
        local_fail_count++;
      }
    }
    if (local_fail_count != 0) {
      fail_count++;
    }
  }
  if (fail_count == 0) {
    cout << "Quantiles algorithm succeeded\n";
  }
  auto stat = fc::get_statistics(durations);
  fc::log("Quantiles algorithm Time to process a range of {:6} elements with "
          "{:6} quantiles:\n"
          "Average : {:10.4f} us,\n"
          "Stdev   : {:10.4f} us,\n"
          "95%     : {:10.4f} us,\n"
          "99%     : {:10.4f} us,\n"
          "99.9%   : {:10.4f} us,\n",
          fc::log_level::all, n, k, stat.average, stat.stdev,
          stat.percentile_95, stat.percentile_99, stat.percentile_999);
}