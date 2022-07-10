#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <interval.h>
#include <iostream>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<float> points;
  mt19937 gen(random_device{}());
  uniform_real_distribution<float> point_dist(0.0f, 10.0f);
  for (int i = 0; i < 10; ++i) {
    points.push_back(point_dist(gen));
  }
  auto itvs = fc::unit_length_intervals(points);
  cout << "Points:\n";
  for (auto point : points) {
    cout << point << ' ';
  }
  cout << '\n';
  cout << "Intervals:\n";
  for (auto [start, finish] : itvs) {
    cout << '(' << start << ", " << finish << ")\n";
  }
  cout << '\n';
}
