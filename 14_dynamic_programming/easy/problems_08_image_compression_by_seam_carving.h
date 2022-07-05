#ifndef __CLRS4_SEAM_CARVING_H__
#define __CLRS4_SEAM_CARVING_H__

#include <algorithm>
#include <bmpreader.h>
#include <cassert>
#include <common.h>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <vector>

namespace frozenca {

using namespace std;

enum class DirSeam {
  None,
  Upleft,
  Up,
  Upright,
};

vector<pair<index_t, index_t>> get_seam(const vector<vector<float>> &d) {
  const auto m = ssize(d);
  assert(m > 2);
  const auto n = ssize(d[0]);
  assert(n > 2);
  vector<vector<float>> D(m, vector<float>(n));
  vector<vector<DirSeam>> S(m, vector<DirSeam>(n, DirSeam::None));
  for (index_t i = 0; i < n; i++) {
    D[0][i] = d[0][i];
  }
  for (index_t i = 1; i < m; i++) {
    for (index_t j = 0; j < n; j++) {
      if (j == 0) {
        if (D[i - 1][j] < D[i - 1][j + 1]) {
          D[i][j] = D[i - 1][j] + d[i][j];
          S[i][j] = DirSeam::Up;
        } else {
          D[i][j] = D[i - 1][j + 1] + d[i][j];
          S[i][j] = DirSeam::Upright;
        }
      } else if (j == n - 1) {
        if (D[i - 1][j - 1] < D[i - 1][j]) {
          D[i][j] = D[i - 1][j - 1] + d[i][j];
          S[i][j] = DirSeam::Upleft;
        } else {
          D[i][j] = D[i - 1][j] + d[i][j];
          S[i][j] = DirSeam::Up;
        }
      } else {
        auto x = min({D[i - 1][j - 1], D[i - 1][j], D[i - 1][j + 1]});
        D[i][j] = x + d[i][j];
        if (x == D[i - 1][j - 1]) {
          S[i][j] = DirSeam::Upleft;
        } else if (x == D[i - 1][j]) {
          S[i][j] = DirSeam::Up;
        } else {
          S[i][j] = DirSeam::Upright;
        }
      }
    }
  }
  index_t q = 0;
  for (index_t j = 0; j < n; j++) {
    if (D[m - 1][j] < D[m - 1][q]) {
      q = j;
    }
  }
  index_t x = m - 1, y = q;
  vector<pair<index_t, index_t>> pos;
  while (x > 0) {
    pos.emplace_back(x, y);
    if (S[x][y] == DirSeam::Upleft) {
      x--;
      y--;
    } else if (S[x][y] == DirSeam::Up) {
      x--;
    } else if (S[x][y] == DirSeam::Upright) {
      x--;
      y++;
    } else {
      assert(false);
    }
  }
  pos.emplace_back(x, y);
  ranges::reverse(pos);
  return pos;
}

void carve_rgb(vector<vector<RGB>> &rgb,
               const vector<pair<index_t, index_t>> &seam) {
  for (auto [i, j] : seam) {
    rgb[i].erase(rgb[i].begin() + j);
  }
}

void compress_by_one(const filesystem::path &bmp_path) {
  array<char, bmp_header_size> header = {0};
  auto rgbs = bmp_to_rgb_grid(bmp_path, header);
  const auto width = ssize(rgbs[0]);
  assert(width > 0);
  auto disruptions = get_disruptions(rgbs);
  auto seam = get_seam(disruptions);
  carve_rgb(rgbs, seam);

  filesystem::path out_path = bmp_path.stem();
  out_path += "_carved";
  out_path += bmp_path.extension();

  *(reinterpret_cast<uint32_t *>(&header[bmp_width_offset])) =
      static_cast<uint32_t>(width - 1);
  write_bmp(out_path, header, rgbs);
}

} // namespace frozenca

#endif //__CLRS4_SEAM_CARVING_H__
