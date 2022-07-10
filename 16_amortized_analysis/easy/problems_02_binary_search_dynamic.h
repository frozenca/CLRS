#ifndef __CLRS4_BINARY_SEARCH_DYNAMIC_H__
#define __CLRS4_BINARY_SEARCH_DYNAMIC_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <iterator>
#include <optional>
#include <ranges>
#include <vector>

namespace frozenca {

using namespace std;

template <Containable T> class DynamicSortedArray {
  vector<vector<T>> A_;
  ptrdiff_t size_ = 0;

  void verify() const {
    index_t total_size = 0;
    const auto n = ssize(A_);
    for (index_t i = 0; i < n; ++i) {
      const auto curr_size = ssize(A_[i]);
      assert(curr_size == 0 || (curr_size == (1ll << i)));
      assert(ranges::is_sorted(A_[i]));
      total_size += curr_size;
    }
    assert(total_size == size_);
  }

public:
  optional<index_t> search(const T &x) const {
    for (index_t i = 0; i < ssize(A_); ++i) {
      if (ranges::binary_search(A_[i], x)) {
        return i;
      }
    }
    return {};
  }

  [[nodiscard]] bool contains(const T &x) const {
    return search(x).has_value();
  }

  void insert(const T &x) {
    vector<T> B{x};
    const auto n = ssize(A_);
    for (index_t i = 0; i < n; ++i) {
      if (A_[i].empty()) {
        A_[i] = move(B);
        size_++;
        verify();
        return;
      } else {
        assert(ssize(A_[i]) == ssize(B));
        vector<T> B2;
        ranges::merge(move(A_[i]), move(B), back_inserter(B2));
        A_[i] = {};
        swap(B, B2);
      }
    }
    A_.push_back(move(B));
    size_++;
    verify();
  }

  bool erase(const T &x) {
    auto i_opt = search(x);
    if (!i_opt.has_value()) {
      return false;
    }
    auto i = *i_opt;
    auto j = distance(A_.begin(), ranges::find_if(A_, [](const auto &cont) {
                        return !cont.empty();
                      }));
    auto x_iter = ranges::find(A_[i], x);
    A_[i].erase(x_iter);
    if (i != j) {
      auto y = A_[j].back();
      A_[i].insert(ranges::lower_bound(A_[i], y), y);
      A_[j].pop_back();
    }
    auto start = 0;
    auto len = 1;
    for (index_t k = 0; k < j; ++k) {
      A_[k] = vector<T>(make_move_iterator(A_[j].begin() + start),
                        make_move_iterator(A_[j].begin() + start + len));
      start = start + len;
      len <<= 1;
    }
    A_[j] = {};
    size_--;
    verify();
    return true;
  }
};

} // namespace frozenca

#endif //__CLRS4_BINARY_SEARCH_DYNAMIC_H__
