#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<fc::index_t> processing_times{3, 5};
    auto [avg_time, tasks] = fc::minimize_average_completion(processing_times);
    cout << "Avg completion time: " << avg_time << '\n';
    for (auto task : tasks) {
      cout << task << ' ';
    }
    cout << '\n';
  }
  {
    vector<pair<fc::index_t, fc::index_t>> processing_times_rel{
        {3, 1}, {5, 2}, {7, 3}};
    auto [avg_time, tasks] =
        fc::minimize_average_completion_release_time(processing_times_rel);
    cout << "Avg completion time: " << avg_time << '\n';
    for (auto task : tasks) {
      cout << task << ' ';
    }
    cout << '\n';
  }
}
