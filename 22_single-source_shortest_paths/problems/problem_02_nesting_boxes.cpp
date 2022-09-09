#include <22_single-source_shortest_paths/inc/easy.h>
#include <cassert>
#include <iostream>
#include <random>


int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  uniform_real_distribution<float> dist(1.0f, 100.0f);
  using Box = fc::Box<float, 5>;
  vector<Box> boxes;
  for (fc::index_t i = 0; i < 200; ++i) {
    Box b;
    for (fc::index_t j = 0; j < 5; ++j) {
      b[j] = dist(gen);
    }
    boxes.push_back(b);
  }

  auto res = fc::longest_nesting_boxes(boxes);
  cout << "Longest nesting boxes:\n";
  for (const auto &box : res) {
    for (auto num : box) {
      cout << num << ' ';
    }
    cout << '\n';
  }
}
