#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  using namespace string_view_literals;

  vector<pair<char, fc::index_t>> char_freq{{'f', 5},  {'e', 9},  {'c', 12},
                                            {'b', 13}, {'d', 16}, {'a', 45}};
  auto root = fc::huffman(char_freq);

  cout << fc::huffman_decode(root.get(), "100011001101"sv);
}
