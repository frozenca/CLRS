#include <vector>
#include <numeric>
#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  constexpr int N = 20;

  vector<int> v(N);
  iota(v.begin(), v.end(), 1);
  fc::print(v);
  fc::randomly_permute_rewrite(v);
  fc::print(v);
  
}