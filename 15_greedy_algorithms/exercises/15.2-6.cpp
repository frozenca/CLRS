#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<pair<float, float>> items{
      {23.0f, 50.5f}, {26.0f, 35.2f}, {20.0f, 45.8f}, {18.0f, 22.0f},
      {32.0f, 35.4f}, {27.0f, 41.4f}, {29.0f, 49.8f}, {26.0f, 54.5f},
      {30.0f, 47.3f}, {27.0f, 54.3f}};

  auto [ratios, value] = fc::knapsack_fractional(items, 67.0f);
  cout << "Value: " << value << '\n';
  cout << "Selected: " << '\n';
  for (auto idx : ratios) {
    cout << idx << ' ';
  }
  cout << '\n';
}
