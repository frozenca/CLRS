#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<pair<fc::index_t, float>> items{
      {23, 50.5f}, {24, 49.5f}, {25, 48.5f}, {26, 47.5f}, {27, 46.5f},
      {28, 45.5f}, {29, 44.5f}, {30, 43.5f}, {31, 42.5f}, {32, 41.5f}};

  {
    auto [selected, value] = fc::knapsack_0_1(items, 67);

    cout << "Value: " << value << '\n';
    cout << "Selected: " << '\n';
    for (auto idx : selected) {
      cout << idx << ' ';
    }
    cout << '\n';
  }
  {
    auto [selected, value] = fc::knapsack_0_1_sorted(items, 67);

    cout << "Value: " << value << '\n';
    cout << "Selected: " << '\n';
    for (auto idx : selected) {
      cout << idx << ' ';
    }
    cout << '\n';
  }
}