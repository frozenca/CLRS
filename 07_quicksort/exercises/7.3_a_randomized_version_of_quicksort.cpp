#include <07_quicksort/inc/easy.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <util.h>


int main() {
  namespace fc = frozenca;
  using namespace std;

  constexpr int length = 3'000;
  mt19937 gen(random_device{}());

  {
    chrono::duration<double, micro> curr_length_duration(0);
    vector<int> v; // random
    generate_n(back_inserter(v), length, ref(gen));
    auto start = chrono::steady_clock::now();
    fc::quicksort(v);
    auto end = chrono::steady_clock::now();
    curr_length_duration += (end - start);
    fc::log("Time to sort a range of {:6} elements : {:10.4f} us\n",
            fc::log_level::all, length, (curr_length_duration.count()));
  }
  {
    chrono::duration<double, micro> curr_length_duration(0);
    vector<int> v(length);
    iota(v.begin(), v.end(), 0);
    ranges::sort(v, ranges::greater{}); // distinct elements, sorted in decreasing order
    auto start = chrono::steady_clock::now();
    fc::quicksort(v);
    auto end = chrono::steady_clock::now();
    curr_length_duration += (end - start);
    fc::log("Time to sort a range of {:6} elements when the range contains distinct elements,\n"
            "sorted in decreasing order : {:10.4f} us\n",
            fc::log_level::all, length, (curr_length_duration.count()));
  }
  {
    // actually performs much worse... surprise?
    chrono::duration<double, micro> curr_length_duration(0);
    vector<int> v(length);
    iota(v.begin(), v.end(), 0);
    ranges::sort(v, ranges::greater{}); // distinct elements, sorted in decreasing order
    auto start = chrono::steady_clock::now();
    fc::randomized_quicksort(v);
    auto end = chrono::steady_clock::now();
    curr_length_duration += (end - start);
    fc::log("Time to sort a range of {:6} elements when the range contains distinct elements,\n"
            "sorted in decreasing order : {:10.4f} us\n",
            fc::log_level::all, length, (curr_length_duration.count()));
  }
}