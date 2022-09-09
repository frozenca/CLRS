#include <22_single-source_shortest_paths/inc/easy.h>
#include <cassert>
#include <iostream>
#include <random>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  uniform_real_distribution<float> dist(0.01f, 10.0f);
  vector<vector<float>> v(20, vector<float>(20));
  for (int i = 0; i < 20; ++i) {
    for (int j = i + 1; j < 20; ++j) {
      auto rate = dist(gen);
      v[i][j] = rate;
      v[j][i] = 1.0f / rate;
    }
  }
  auto res = fc::arbitrage(v);
  if (res) {
    cout << "Arbitrage is not possible\n";
  } else {
    cout << "Arbitrage is possible\n";
  }
}
