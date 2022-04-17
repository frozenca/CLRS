#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;
  for (int i = 0; i < 30; i++) {
    cout << fc::random_gen(8, 13) << ' ';
  }
}