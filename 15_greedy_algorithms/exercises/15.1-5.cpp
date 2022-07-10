#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<pair<fc::Interval, float>> v{
      {{1.0f, 4.0f}, 1.0f},  {{3.0f, 5.0f}, 1.2f},  {{0.0f, 6.0f}, 1.4f},
      {{5.0f, 7.0f}, 1.6f},  {{3.0f, 9.0f}, 1.8f},  {{5.0f, 9.0f}, 2.0f},
      {{6.0f, 10.0f}, 2.2f}, {{7.0f, 11.0f}, 2.4f}, {{8.0f, 12.0f}, 2.6f},
      {{2.0f, 14.0f}, 2.8f}, {{12.0f, 16.0f}, 3.0f}};

  auto A = fc::dp_activity_selector_valued(v);
  for (auto [b, e] : A) {
    cout << '(' << b << ' ' << e << ")\n";
  }
  cout << '\n';
}