#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <point.h>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::Point2d> p;
  p.emplace_back(0.0f, 0.0f);
  p.emplace_back(1.0f, 6.0f);
  p.emplace_back(2.0f, 3.0f);
  p.emplace_back(5.0f, 2.0f);
  p.emplace_back(6.0f, 5.0f);
  p.emplace_back(7.0f, 1.0f);
  p.emplace_back(8.0f, 4.0f);

  fc::bitonic_euclidean_tsp(p);
}