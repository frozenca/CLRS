#include <bit>
#include <bitset>
#include <cassert>
#include <common.h>
#include <iostream>

namespace fc = frozenca;
using namespace std;

fc::index_t matrix_2d_index_to_1d_index(fc::index_t i, fc::index_t j,
                                        fc::index_t m, fc::index_t n) {
  assert(has_single_bit(static_cast<fc::uindex_t>(m)) &&
         has_single_bit(static_cast<fc::index_t>(n)));
  const auto block_size = (m / 2) * (n / 2);
  const auto block_row_idx = i / (m / 2);
  const auto block_row_inside_idx = i % (m / 2);
  const auto block_col_idx = j / (n / 2);
  const auto block_col_inside_idx = j % (m / 2);
  const auto block_idx = block_row_idx * 2 + block_col_idx;
  const auto block_inside_idx =
      block_row_inside_idx * (n / 2) + block_col_inside_idx;
  const auto final_idx = block_idx * block_size + block_inside_idx;
  return final_idx;
}

int main() {
  constexpr fc::index_t m = 8;
  constexpr fc::index_t n = 8;
  for (fc::index_t i = 0; i < m; ++i) {
    for (fc::index_t j = 0; j < n; ++j) {
      cout << "1D index of (" << i << ", " << j << ") is "
           << matrix_2d_index_to_1d_index(i, j, m, n) << "\n";
    }
  }
}