#ifndef __CLRS4_SEARCH_COMPACT_LIST_H__
#define __CLRS4_SEARCH_COMPACT_LIST_H__

#include <array>
#include <common.h>
#include <random>

namespace frozenca {

using namespace std;

template <typename T, size_t N = arr_default_length> class CompactList {

  static constexpr index_t NIL = -1;

  array<T, N> key_ = {0};
  array<index_t, N> next_ = {0};
  index_t head_ = NIL;
  array<bool, N> filled_ = {0};

  index_t find_empty() const {
    for (auto i = 0; i < N; ++i) {
      if (!filled_[i]) {
        return i;
      }
    }
    return NIL;
  }

public:
  index_t list_search(const T &k) const {
    auto curr = head_;
    while (curr != NIL && key_[curr] < k) {
      curr = next_[curr];
    }
    if (curr == NIL || key_[curr] > k) {
      return NIL;
    } else {
      return curr;
    }
  }

  index_t list_search_compact(const T &k) const {
    assert(find_empty() == NIL);
    auto curr = head_;
    mt19937 gen(random_device{}());
    uniform_int_distribution<index_t> dist(0, N - 1);
    while (curr != NIL && key_[curr] < k) {
      auto rd = dist(gen);
      if (key_[curr] < key_[rd] && key_[rd] < k) {
        curr = rd;
        if (key_[curr] == k) {
          return curr;
        }
      }
      curr = next_[curr];
    }
    if (curr == NIL || key_[curr] > k) {
      return NIL;
    } else {
      return curr;
    }
  }

  index_t list_search_compact2(const T &k, int t = 20) const {
    assert(find_empty() == NIL);
    auto curr = head_;
    mt19937 gen(random_device{}());
    uniform_int_distribution<index_t> dist(0, N - 1);

    for (int q = 0; q < t; ++q) {
      auto rd = dist(gen);
      if (key_[curr] < key_[rd] && key_[rd] < k) {
        curr = rd;
        if (key_[curr] == k) {
          return curr;
        }
      }
    }
    while (curr != NIL && key_[curr] < k) {
      curr = next_[curr];
    }
    if (curr == NIL || key_[curr] > k) {
      return NIL;
    } else {
      return curr;
    }
  }

  void list_insert(const T &k) {
    if (head_ == NIL) { // empty
      head_ = 0;
      key_[head_] = k;
      next_[head_] = NIL;
      filled_[head_] = true;
      return;
    }
    auto curr = head_;
    auto prev = NIL;
    while (curr != NIL && key_[curr] < k) {
      prev = curr;
      curr = next_[curr];
    }
    // invariant: key[prev] < k <= key[curr]
    if (curr == NIL) { // key[tail] < k
      auto empty_index = find_empty();
      if (empty_index == NIL) { // all filled, cannot insert
        return;
      } else {
        assert(prev != NIL);
        key_[empty_index] = k;
        next_[prev] = empty_index;
        next_[empty_index] = NIL;
        filled_[empty_index] = true;
      }
    } else if (key_[curr] >= k) {
      if (prev == NIL) { // curr is head_
        assert(curr == head_);
        auto empty_index = find_empty();
        if (empty_index == NIL) { // all filled, cannot insert
          return;
        } else { // promote this as new head
          key_[empty_index] = k;
          next_[empty_index] = curr;
          filled_[empty_index] = true;
          head_ = empty_index;
        }
      } else { // insert between prev and curr
        auto empty_index = find_empty();
        if (empty_index == NIL) { // all filled, cannot insert
          return;
        } else {
          key_[empty_index] = k;
          next_[prev] = empty_index;
          next_[empty_index] = curr;
          filled_[empty_index] = true;
        }
      }
    }
  }

  void list_delete(const T &k) {
    auto curr = head_;
    auto prev = NIL;
    while (curr != NIL && key_[curr] < k) {
      prev = curr;
      curr = next_[curr];
    }
    // invariant: key[prev] < k <= key[curr]
    if (curr == NIL || key_[curr] > k) { // k not in the list
      return;
    }
    assert(key_[curr] == k);
    if (prev == NIL) {
      assert(curr == head_);
      head_ = next_[curr];
      key_[curr] = T{};
      next_[curr] = 0;
      filled_[curr] = false;
    } else {
      auto next = next_[curr];
      key_[curr] = T{};
      next_[curr] = 0;
      filled_[curr] = false;
      next_[prev] = next;
    }
  }

  friend ostream &operator<<(ostream &os, const CompactList &l) {
    auto x = l.head_;
    while (x != l.NIL) {
      os << l.key_[x] << ' ';
      x = l.next_[x];
    }
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_SEARCH_COMPACT_LIST_H__
