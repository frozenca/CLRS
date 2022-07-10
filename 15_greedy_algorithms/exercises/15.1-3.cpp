#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<fc::Interval> v{{1.0f, 5.0f}, {4.0f, 7.0f}, {6.0f, 11.0f}};

    auto AG = fc::greedy_activity_selector(v);
    for (auto [b, e] : AG) {
      cout << '(' << b << ' ' << e << ")\n";
    }
    cout << '\n';

    auto A = fc::greedy_activity_selector_compatible_least_duration(v);
    for (auto [b, e] : A) {
      cout << '(' << b << ' ' << e << ")\n";
    }
    cout << '\n';
  }
  {
    vector<fc::Interval> v{{0.0f, 2.0f},   {1.0f, 3.0f}, {1.01f, 3.01f},
                           {1.02f, 3.02f}, {2.0f, 5.0f}, {4.0f, 6.0f},
                           {5.0f, 7.0f},   {6.0f, 8.0f}, {6.01f, 8.01f},
                           {6.02f, 8.02f}, {7.0f, 9.0f}};

    auto AG = fc::greedy_activity_selector(v);
    for (auto [b, e] : AG) {
      cout << '(' << b << ' ' << e << ")\n";
    }
    cout << '\n';

    auto A = fc::greedy_activity_selector_overlaps_fewest(v);
    for (auto [b, e] : A) {
      cout << '(' << b << ' ' << e << ")\n";
    }
    cout << '\n';
  }
  {
    vector<fc::Interval> v{{2.0f, 3.0f}, {4.0f, 5.0f}, {1.0f, 6.0f}};

    auto AG = fc::greedy_activity_selector(v);
    for (auto [b, e] : AG) {
      cout << '(' << b << ' ' << e << ")\n";
    }
    cout << '\n';

    auto A = fc::greedy_activity_selector_compatible_earliest(v);
    for (auto [b, e] : A) {
      cout << '(' << b << ' ' << e << ")\n";
    }
    cout << '\n';
  }
}
