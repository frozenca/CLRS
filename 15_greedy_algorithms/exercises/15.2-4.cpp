#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<float> highway;
  mt19937 gen(random_device{}());
  uniform_real_distribution<float> stop_dist(0.0f, 5.0f);
  for (int i = 0; i < 10; ++i) {
    highway.push_back(stop_dist(gen));
  }
  auto stops = fc::skating(highway, 1.0f);
  cout << "Stops:\n";
  for (auto stop : highway) {
    cout << stop << ' ';
  }
  cout << '\n';
  cout << "Actual stops:\n";
  for (auto idx : stops) {
    cout << highway[idx] << ' ';
  }
  cout << '\n';
}