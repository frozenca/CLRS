#include <05_probabilistic_analysis_and_randomized_algorithms/inc/easy.h>
#include <map>
#include <numeric>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  constexpr int N = 3;
  constexpr int trials = 2'000;
  map<vector<int>, int> freq;
  for (int i = 0; i < trials; ++i) {
    vector<int> v(N);
    iota(v.begin(), v.end(), 1);
    fc::randomly_permute_with_all(v);
    freq[v]++;
  }
  for (const auto& [v, count] : freq) {
    fc::print(v);
    cout << count / static_cast<float>(trials) << '\n';
  }
}