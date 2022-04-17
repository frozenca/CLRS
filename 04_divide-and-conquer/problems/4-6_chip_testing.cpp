#include <cassert>
#include <common.h>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<int> chips;
  constexpr fc::index_t num_chips = 30;
  chips.resize(num_chips);
  constexpr fc::index_t num_good_chips = 1 + num_chips / 2;
  for (fc::index_t i = 0; i < num_good_chips; ++i) {
    chips[i] = 1;
  }

  mt19937 gen(random_device{}());
  bernoulli_distribution d(0.5);

  shuffle(chips.begin(), chips.end(), gen);
  vector<int> predictions(chips.size());
  for (fc::index_t i = 0; i < num_chips - 1; ++i) {
    int resA = chips[i] ? chips[i + 1] : d(gen);
    int resB = chips[i + 1] ? chips[i] : d(gen);
    if (resA && resB) {   // report say that both is good.
      predictions[i] = 1; // take the first, discard the second
    } else {
      ++i; // discard both of them
    }
  }
  fc::index_t last_good = 0;
  for (fc::index_t i = num_chips - 1; i >= 0; --i) {
    if (predictions[i]) {
      last_good = i;
      break;
    }
  }
  for (fc::index_t i = 0; i < num_chips; ++i) {
    if (i == last_good) {
      continue;
    }
    int resA = chips[last_good] ? chips[i] : d(gen);
    predictions[i] = resA;
  }
  if (predictions == chips) {
    cout << "Chip testing succeeded\n";
  } else {
    cout << "Chip testing failed\n";
  }
}