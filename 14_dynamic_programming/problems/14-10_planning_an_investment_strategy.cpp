#include <14_dynamic_programming/inc/easy.h>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  uniform_real_distribution<float> stock_dist(-0.15f, 0.25f);
  uniform_real_distribution<float> estate_dist(0.05f, 0.1f);
  uniform_real_distribution<float> bitcoin_dist(-0.5f, 0.5f);

  float d = 100'000.0f;
  vector<vector<float>> r(3, vector<float>(10));

  for (fc::index_t i = 0; i < 10; ++i) {
    r[0][i] = stock_dist(gen);
    r[1][i] = estate_dist(gen);
    r[2][i] = bitcoin_dist(gen);
  }
  cout << "Stock return rate:\n";
  for (auto rate : r[0]) {
    cout << rate << ' ';
  }
  cout << '\n';
  cout << "Estate return rate:\n";
  for (auto rate : r[1]) {
    cout << rate << ' ';
  }
  cout << '\n';
  cout << "Cryptocurrency return rate:\n";
  for (auto rate : r[2]) {
    cout << rate << ' ';
  }
  cout << '\n';

  float f_1 = 100.0f;
  float f_2 = 500.0f;

  fc::invest(d, r, f_1, f_2);
}