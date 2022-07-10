#include <17_augmenting_data_structures/inc/easy.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  constexpr int N = 5;

  vector<pair<float, float>> chords;
  for (int i = 0; i < N; i++) {
    chords.emplace_back(i * 1.0f / static_cast<float>(2 * N),
                        0.5f + i * 1.0f / static_cast<float>(2 * N));
  }

  cout << fc::num_pairs_intersecting(chords)
       << '\n'; // must be equal to N * (N - 1) / 2
}
