#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<float> p{0.15f, 0.10f, 0.05f, 0.10f, 0.20f};
  vector<float> q{0.05f, 0.10f, 0.05f, 0.05f, 0.05f, 0.10f};
  fc::construct_optimal_bst(p, q);
}