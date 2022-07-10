#ifndef __CLRS4_OFFLINE_CACHING_H__
#define __CLRS4_OFFLINE_CACHING_H__

#include <bitset>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

inline constexpr size_t num_chars = 32UL;

using cache_t = bitset<num_chars>;

index_t offline_caching_memo(const vector<char> &blocks,
                             vector<unordered_map<cache_t, index_t>> &miss,
                             const size_t K, index_t idx, cache_t curr_cache) {
  if (miss[idx].contains(curr_cache)) {
    return miss[idx][curr_cache];
  }
  auto &res = miss[idx][curr_cache];
  const auto n = ssize(blocks);
  auto b_i = blocks[idx] - 'a';
  if (idx == n - 1) {
    res = (curr_cache[b_i] == 1) ? 0 : 1;
    return res;
  }
  if (curr_cache[b_i]) {
    res = offline_caching_memo(blocks, miss, K, idx + 1, curr_cache);
    return res;
  } else {
    auto val = numeric_limits<index_t>::max();
    if (curr_cache.count() < K) {
      curr_cache[b_i] = 1;
      res = 1 + offline_caching_memo(blocks, miss, K, idx + 1, curr_cache);
      return res;
    } else {
      for (size_t i = 0; i < num_chars; ++i) {
        if (!curr_cache[i]) {
          continue;
        }
        auto next_cache = curr_cache;
        next_cache[i] = 0;
        next_cache[b_i] = 1;
        auto curr_val =
            offline_caching_memo(blocks, miss, K, idx + 1, next_cache);
        if (curr_val < val) {
          val = curr_val;
        }
      }
      res = val + 1;
      return res;
    }
  }
}

index_t offline_caching(const vector<char> &blocks, size_t K) {
  cache_t init = {0};
  const auto n = ssize(blocks);
  vector<unordered_map<cache_t, index_t>> miss(n);
  return offline_caching_memo(blocks, miss, K, 0, init);
}

using BlockIter = vector<char>::const_iterator;

void offline_caching_furthest_in_future_impl(cache_t &C, BlockIter first,
                                             BlockIter last, size_t K) {
  if (first == last) {
    return;
  }
  auto b_i = *first - 'a';
  if (C[b_i]) {
    cout << "Cache hit\n";
    offline_caching_furthest_in_future_impl(C, next(first), last, K);
  } else {
    cout << "Cache miss\n";
    if (C.count() < K) {
      C[b_i] = 1;
      offline_caching_furthest_in_future_impl(C, next(first), last, K);
    } else {
      index_t dist = 0;
      index_t to_evict = -1;
      for (size_t i = 0; i < num_chars; ++i) {
        if (!C[i]) {
          continue;
        }
        auto curr_dist = ranges::distance(
            first, ranges::find(first, last, 'a' + static_cast<char>(i)));
        if (curr_dist > dist) {
          dist = curr_dist;
          to_evict = i;
        }
      }
      cout << "Evict " << static_cast<char>('a' + static_cast<char>(to_evict))
           << '\n';
      C[to_evict] = 0;
      C[b_i] = 1;
      offline_caching_furthest_in_future_impl(C, next(first), last, K);
    }
  }
}

void offline_caching_furthest_in_future(cache_t &C, const vector<char> &blocks,
                                        size_t K) {
  offline_caching_furthest_in_future_impl(C, blocks.begin(), blocks.end(), K);
}

void offline_caching_lru_impl(cache_t &C, BlockIter curr, BlockIter first,
                              BlockIter last, size_t K) {
  if (curr == last) {
    return;
  }
  auto b_i = *curr - 'a';
  if (C[b_i]) {
    cout << "Cache hit\n";
    offline_caching_lru_impl(C, next(curr), first, last, K);
  } else {
    cout << "Cache miss\n";
    if (C.count() < K) {
      C[b_i] = 1;
      offline_caching_lru_impl(C, next(curr), first, last, K);
    } else {
      index_t dist = 0;
      index_t to_evict = -1;
      for (size_t i = 0; i < num_chars; ++i) {
        if (!C[i]) {
          continue;
        }
        auto curr_dist = ranges::distance(
            first, ranges::find(first, curr, 'a' + static_cast<char>(i)));
        if (curr_dist > dist) {
          dist = curr_dist;
          to_evict = i;
        }
      }
      cout << "Evict " << static_cast<char>('a' + static_cast<char>(to_evict))
           << '\n';
      C[to_evict] = 0;
      C[b_i] = 1;
      offline_caching_lru_impl(C, next(curr), first, last, K);
    }
  }
}

void offline_caching_lru(cache_t &C, const vector<char> &blocks, size_t K) {
  offline_caching_lru_impl(C, blocks.begin(), blocks.begin(), blocks.end(), K);
}

} // namespace frozenca

#endif //__CLRS4_OFFLINE_CACHING_H__
