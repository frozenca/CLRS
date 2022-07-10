#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<pair<fc::index_t, float>> items{
      {23, 50.5f}, {26, 35.2f}, {20, 45.8f}, {18, 22.0f}, {32, 35.4f},
      {27, 41.4f}, {29, 49.8f}, {26, 54.5f}, {30, 47.3f}, {27, 54.3f}};

  auto [selected, value] = fc::knapsack_0_1(items, 67);

  cout << "Value: " << value << '\n';
  cout << "Selected: " << '\n';
  for (auto idx : selected) {
    cout << idx << ' ';
  }
  cout << '\n';
}
