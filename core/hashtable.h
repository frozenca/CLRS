#ifndef __CLRS4_HASH_TABLE_H__
#define __CLRS4_HASH_TABLE_H__

#include <array>
#include <common.h>
#include <concepts>
#include <functional>
#include <iostream>
#include <linkedlist.h>
#include <random>
#include <type_traits>
#include <utility>

namespace frozenca {

using namespace std;

template <typename H, typename K>
concept Hashable = requires(H h, K k) {
  { h(k) } -> convertible_to<size_t>;
};

template <typename K, size_t N = hashtable_default_length>
struct HashFunctionDefault {
  size_t operator()(const K &key) const { return hash<K>{}(key) % N; }
};

template <typename K, typename V,
          typename H = HashFunctionDefault<K, hashtable_default_length>,
          size_t N = hashtable_default_length>
requires Hashable<H, K>
class HashTable {
  using L = LinkedPairList<K, V>;
  H h = H{};
  array<L, N> T = {};

public:
  void chained_hash_insert(const pair<K, V> &x) {
    T[h(x.first)].list_prepend(x);
  }

  V chained_hash_search(const K &key) const {
    return T[h(key)].list_search(key);
  }

  void chained_hash_delete(const K &key) { T[h(key)].list_delete(key); }

  friend ostream &operator<<(ostream &os, const HashTable &h) {
    for (const auto &l : h.T) {
      os << l;
    }
    return os;
  }

  V get_random_value() const {
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(0, N - 1);
    auto ind = dist(gen);
    int trial = 0;
    while (!T[ind].head() && trial < N) {
      ind = dist(gen);
      trial++;
    }
    if (!T[ind].head()) {
      return V{};
    }
    uniform_int_distribution<> dist2(0, T[ind].length() - 1);
    auto bucket_ind = dist2(gen);
    return T[ind].get_nth(bucket_ind);
  }
};

} // namespace frozenca

#endif //__CLRS4_HASH_TABLE_H__
