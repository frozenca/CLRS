#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::Interval> v{{1.0f, 4.0f},  {3.0f, 5.0f},  {0.0f, 6.0f},
                         {5.0f, 7.0f},  {3.0f, 9.0f},  {5.0f, 9.0f},
                         {6.0f, 10.0f}, {7.0f, 11.0f}, {8.0f, 12.0f},
                         {2.0f, 14.0f}, {12.0f, 16.0f}};

  auto lecture_map = fc::schedule_lectures(v);

  for (const auto &[idx, hall] : lecture_map) {
    cout << '(' << v[idx].first_ << ", " << v[idx].second_ << "): " << hall
         << '\n';
  }
}