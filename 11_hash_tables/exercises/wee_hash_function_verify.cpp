#include <11_hash_tables/inc/hard.h>
#include <iostream>
#include <limits>
#include <random>
#include <tuple>
#include <unordered_set>


int main() {

  namespace fc = frozenca;
  using namespace std;
  constexpr int num_trials = 1'000'000;

  mt19937 gen(random_device{}());
  uniform_int_distribution<size_t> dist(0, numeric_limits<size_t>::max());

  // std::hash
  {
    unordered_set<size_t> s;

    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        s.insert(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("std::hash time to insert {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        ignore = s.contains(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("std::hash time to retrieve {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        s.erase(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("std::hash time to erase {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
  }

  
  // Wee hash function
  {
    using OurWee = fc::hard::Wee<123, 0, 64, 4>;
    unordered_set<size_t, OurWee> s;

    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        s.insert(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("Wee time to insert {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        ignore = s.contains(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("Wee time to retrieve {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
    {
      vector<float> durations;
      for (int i = 0; i < num_trials; ++i) {
        auto num = dist(gen);
        auto start = chrono::steady_clock::now();
        s.erase(num);
        auto end = chrono::steady_clock::now();
        durations.push_back(
            chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                .count());
      }
      auto stat = fc::get_statistics(durations);
      fc::log("Wee time to erase {:6} elements:\n"
              "Average : {:10.4f} us,\n"
              "Stdev   : {:10.4f} us,\n"
              "95%     : {:10.4f} us,\n"
              "99%     : {:10.4f} us,\n"
              "99.9%   : {:10.4f} us,\n",
              fc::log_level::all, num_trials, stat.average, stat.stdev,
              stat.percentile_95, stat.percentile_99, stat.percentile_999);
    }
  }
}