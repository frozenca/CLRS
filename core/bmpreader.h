#ifndef __CLRS4_BMP_READER_H__
#define __CLRS4_BMP_READER_H__

#include <array>
#include <cassert>
#include <cmath>
#include <common.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace frozenca {

using namespace std;

struct RGB {
  unsigned char r_ = 0;
  unsigned char g_ = 0;
  unsigned char b_ = 0;
};

float disruption(const RGB &pixel1, const RGB &pixel2) {
  float r_diff =
      static_cast<float>((pixel1.r_ >= pixel2.r_) ? (pixel1.r_ - pixel2.r_)
                                                  : (pixel2.r_ - pixel1.r_));
  float g_diff =
      static_cast<float>((pixel1.g_ >= pixel2.g_) ? (pixel1.g_ - pixel2.g_)
                                                  : (pixel2.g_ - pixel1.g_));
  float b_diff =
      static_cast<float>((pixel1.b_ >= pixel2.b_) ? (pixel1.b_ - pixel2.b_)
                                                  : (pixel2.b_ - pixel1.b_));
  return hypot(r_diff, g_diff, b_diff);
}

vector<vector<float>> get_disruptions(const vector<vector<RGB>> &rgbs) {
  const auto width = ssize(rgbs);
  assert(width > 0);
  const auto height = ssize(rgbs[0]);
  assert(height > 0);
  vector<vector<float>> disruptions(width, vector<float>(height));

  vector<pair<index_t, index_t>> dirs{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (index_t i = 0; i < width; ++i) {
    for (index_t j = 0; j < height; ++j) {
      index_t n_count = 0;
      float disruptive = 0.0f;
      for (const auto &[di, dj] : dirs) {
        index_t next_i = i + di;
        index_t next_j = j + dj;
        if (next_i < 0 || next_i >= width || next_j < 0 || next_j >= height) {
          continue;
        }
        n_count++;
        disruptive += disruption(rgbs[i][j], rgbs[next_i][next_j]);
      }
      if (n_count > 0) {
        disruptions[i][j] = disruptive / n_count;
      } else {
        disruptions[i][j] = 0.0f;
      }
    }
  }
  return disruptions;
}

inline constexpr size_t bmp_header_size = 54UL;
inline constexpr size_t bmp_width_offset = 18UL;
inline constexpr size_t bmp_height_offset = 22UL;

// vector<vector<T>> is a common code smell,
// but let's go simple... let's not pass additional width/height as return
// values

vector<vector<RGB>> bmp_to_rgb_grid(const filesystem::path &bmp_path,
                                    array<char, bmp_header_size> &header) {
  ifstream img(bmp_path, ios::binary | ios::in);

  if (!img.read(&header[0], bmp_header_size)) {
    throw invalid_argument("not a bitmap image\n");
  }
  index_t width = static_cast<index_t>(
      *(reinterpret_cast<uint32_t *>(&header[bmp_width_offset])));
  index_t height = static_cast<index_t>(
      *(reinterpret_cast<uint32_t *>(&header[bmp_height_offset])));
  if (width <= 0 || height <= 0) {
    throw invalid_argument("invalid header\n");
  }
  index_t stride =
      static_cast<index_t>(static_cast<uint32_t>(width * 3 + 3) & ~(3u));
  index_t padding = stride - width * 3;

  cout << "Read success\n";
  cout << "Width: " << width << '\n';
  cout << "Height: " << height << '\n';
  cout << "Stride: " << stride << '\n';
  cout << "Padding: " << padding << '\n';

  vector<vector<RGB>> rgbs(height, vector<RGB>(width));
  auto image_bytes = stride * height;
  vector<char> rgb_buf(image_bytes);
  if (!img.read(&rgb_buf[0], image_bytes)) {
    throw invalid_argument("rgb read fail\n");
  }
  for (index_t i = 0; i < height; ++i) {
    for (index_t j = 0; j < width; ++j) {
      index_t offset = i * (stride / 3) + j;
      rgbs[i][j].r_ = static_cast<unsigned char>(rgb_buf[offset * 3]);
      rgbs[i][j].g_ = static_cast<unsigned char>(rgb_buf[offset * 3 + 1]);
      rgbs[i][j].b_ = static_cast<unsigned char>(rgb_buf[offset * 3 + 2]);
    }
  }

  return rgbs;
}

vector<vector<RGB>> bmp_to_rgb_grid(const filesystem::path &bmp_path) {
  array<char, bmp_header_size> header = {0};
  return bmp_to_rgb_grid(bmp_path, header);
}

void write_bmp(const filesystem::path &bmp_path,
               const array<char, bmp_header_size> &header,
               const vector<vector<RGB>> &rgbs) {
  ofstream img(bmp_path, ios::binary | ios::out);
  if (!img.write(&header[0], bmp_header_size)) {
    throw invalid_argument("invalid header\n");
  }
  const auto height = ssize(rgbs);
  assert(height > 0);
  const auto width = ssize(rgbs[0]);
  assert(width > 0);
  index_t stride =
      static_cast<index_t>(static_cast<uint32_t>(width * 3 + 3) & ~(3u));
  index_t padding = stride - width * 3;

  cout << "Writing...\n";
  cout << "Width: " << width << '\n';
  cout << "Height: " << height << '\n';
  cout << "Stride: " << stride << '\n';
  cout << "Padding: " << padding << '\n';
  auto image_bytes = stride * height;
  vector<char> rgb_buf(image_bytes);
  for (index_t i = 0; i < height; ++i) {
    for (index_t j = 0; j < width; ++j) {
      index_t offset = i * (stride / 3) + j;
      rgb_buf[offset * 3] = static_cast<char>(rgbs[i][j].r_);
      rgb_buf[offset * 3 + 1] = static_cast<char>(rgbs[i][j].g_);
      rgb_buf[offset * 3 + 2] = static_cast<char>(rgbs[i][j].b_);
    }
  }
  if (!img.write(&rgb_buf[0], image_bytes)) {
    throw invalid_argument("rgb write fail\n");
  }
}

} // namespace frozenca

#endif //__CLRS4_BMP_READER_H__
