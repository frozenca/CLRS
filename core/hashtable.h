#ifndef __CLRS4_HASH_TABLE_H__
#define __CLRS4_HASH_TABLE_H__

#include <bit>
#include <common.h>
#include <hashfunction.h>
#include <iomanip>
#include <iostream>
#include <linkedlist.h>
#include <vector>

namespace frozenca::hard {

using namespace std;

namespace detail {

template <Containable K, typename V, typename Hasher, bool AllowDup>
requires Hashable<Hasher, K>
class HashTable {
  // invariant: V is either K or pair<const K, Value> for some Value type.
  static constexpr bool is_set_ = is_same_v<K, V>;

  using Node = ListNode<V>;

  LinkedList<V> values_;

  // invariant: K cannot be mutated
  // so if V is K, ListIter is const iterator.
  // if V is pair<const K, value>, ListIter is non-const iterator (but only
  // value can be mutated)
  using ListIter = ListIterator<V, is_set_>;

  // invariant: buckets_.size() == 0 or must be a power of 2
  // each "bucket" is a pair of two ListIter [begin, end)
  // invariant: begin <= end, and all iterators between begin and end
  // points to elements with the same hash value, with keys in sorted order.
  using Buckets = vector<ListIter>;
  Buckets buckets_;

  // invariant: buckets.size() == 0 or bucket_mask_ == buckets_.size() - 1
  size_t bucket_mask_ = 0;

  static constexpr size_t growth_factor_early_ = 4UL;
  static constexpr size_t growth_factor_ = 2UL;

  // ideally, this should be equal to half of the initial nonempty capacity of std::vector...
  static constexpr size_t initial_nonempty_bucket_count_ = 8UL;
  static constexpr size_t bucket_count_threshold_ = 128UL;

  static constexpr float max_load_factor_ = 1.0f;

  struct KeyProj {
    K operator()(const V &value) const noexcept {
      if constexpr (is_set_) {
        return value;
      } else {
        return value.first;
      }
    }
  };

  size_t get_hash(const K &key) const noexcept { return Hasher{}(key); }

  size_t bucket_from_key(const K &key) const noexcept {
    return get_hash(key) & bucket_mask_;
  }

  size_t bucket(const V &value) const noexcept {
    return bucket_from_key(KeyProj{}(value));
  }

public:
  using key_type = K;
  using value_type = V;
  using reference_type = V &;
  using const_reference_type = const V &;
  using iterator_type = ListIter;
  using const_iterator_type = ListIterator<V, true>;
  // hash tables have no reverse iterators!

  HashTable() = default;
  ~HashTable() = default;

private:
  void clone_buckets(const Buckets &other_buckets) {
    // clone bucket iterator pairs one by one
    // precondition: values_ are already cloned
    auto this_bucket_curr = values_.begin();

    buckets_.clear();
    buckets_.reserve(other_buckets.size());
    for (size_t i = 0; i < other_buckets.size(); i += 2) {
      auto this_bucket_begin = this_bucket_curr;
      auto other_bucket_curr = other_buckets[i];
      while (other_bucket_curr != other_buckets[i + 1]) {
        ++this_bucket_curr;
        ++other_bucket_curr;
      }
      auto this_bucket_end = this_bucket_curr;
      buckets_.push_back(this_bucket_begin);
      buckets_.push_back(this_bucket_end);
    }
    assert(this_bucket_curr == values_.end());
  }

public:
  HashTable(const HashTable &other) : values_{other.values_} {
    clone_buckets(other.buckets_);
  }
  HashTable &operator=(const HashTable &other) {
    values_ = other.values_;
    clone_buckets(other.buckets_);
    return *this;
  }
  HashTable(HashTable &&other) noexcept = default;
  HashTable &operator=(HashTable &&other) noexcept = default;

  [[nodiscard]] iterator_type begin() noexcept {
    return iterator_type(values_.begin());
  }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_iterator_type(values_.begin());
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_iterator_type(values_.cbegin());
  }

  [[nodiscard]] iterator_type end() noexcept {
    return iterator_type(values_.end());
  }

  [[nodiscard]] const_iterator_type end() const noexcept {
    return const_iterator_type(values_.end());
  }

  [[nodiscard]] const_iterator_type cend() const noexcept {
    return const_iterator_type(values_.end());
  }

  [[nodiscard]] bool empty() const noexcept { return values_.empty(); }

  [[nodiscard]] ptrdiff_t size() const noexcept { return values_.size(); }

  void clear() noexcept {
    values_.clear();
    buckets_.clear();
    bucket_mask_ = 0;
  }

  size_t bucket_count() const noexcept { return (buckets_.size() >> 1); }

  float load_factor() const noexcept {
    if (empty()) {
      return 0.0f;
    }
    return static_cast<float>(size()) / static_cast<float>(buckets_.size());
  }

  float max_load_factor() const noexcept { return max_load_factor_; }

private:
  [[nodiscard]] pair<bool, size_t> need_rehash() const noexcept {
    if (buckets_.empty()) {
      return {true, initial_nonempty_bucket_count_};
    } else {
      float next_load_factor =
          static_cast<float>(size() + 1) / static_cast<float>(bucket_count());
      if (next_load_factor >= max_load_factor_) {
        if (bucket_count() < bucket_count_threshold_) {
          return {true, bucket_count() * growth_factor_early_};
        } else {
          return {true, bucket_count() * growth_factor_};
        }
      } else {
        return {false, 0};
      }
    }
  }

  void rehash(size_t next_bucket_size) {
    assert(has_single_bit(next_bucket_size));
    buckets_.assign(next_bucket_size << 1, end());

    bucket_mask_ = next_bucket_size - 1;

    auto curr_it = begin();
    auto end_it = end();

    while (curr_it != end_it) {
      auto value = *curr_it;
      auto bucket_index = bucket(value);
      auto next_it = next(curr_it);

      auto &lo = buckets_[(bucket_index << 1)];
      auto &hi = buckets_[(bucket_index << 1) + 1];

      if (lo == hi) {
        assert(lo == end() && hi == end());
        lo = curr_it;
        hi = next_it;
        curr_it = next_it;
        continue;
      }

      // insertion point
      auto it = ranges::lower_bound(lo, hi, value, less<K>{}, KeyProj{});
      assert(it != next_it);
      if (it == curr_it) {
        assert(it == hi && hi != end());
        ++hi;
      } else if (it == lo) {
        assert(it != begin());
        auto &prev_hi = buckets_[(bucket(*prev(it)) << 1) + 1];
        assert(prev_hi == lo);
        values_.splice(it, curr_it, next_it);
        lo = curr_it;
        prev_hi = lo;
      } else {
        assert(hi != end());
        auto &next_hi = buckets_[(bucket(*hi) << 1) + 1];
        if (next_hi == curr_it) {
          ++next_hi;
        }
        values_.splice(it, curr_it, next_it);
      }

      curr_it = next_it;
    }
  }

  iterator_type find_key_in_bucket(size_t bucket_index,
                                   const K &key) const noexcept {
    auto value = [&]() {
      if constexpr (is_set_) {
        return key;
      } else {
        return V{key, key};
      }
    };
    return find_in_bucket(bucket_index, value());
  }

  iterator_type find_in_bucket(size_t bucket_index,
                               const V &value) const noexcept {
    auto lo = buckets_[bucket_index << 1];
    auto hi = buckets_[(bucket_index << 1) + 1];
    auto it = ranges::lower_bound(lo, hi, value, less<K>{}, KeyProj{});
    if (it == hi || (value != *it)) {
      return end();
    } else {
      return it;
    }
  }

  size_t erase_key_from_bucket(size_t bucket_index, const K &key) {
    auto lo = buckets_[bucket_index << 1];
    auto hi = buckets_[(bucket_index << 1) + 1];
    auto value = [&]() {
      if constexpr (is_set_) {
        return key;
      } else {
        return V{key, key};
      }
    };
    auto it = ranges::lower_bound(lo, hi, value(), less<K>{}, KeyProj{});
    if (it == hi || (value() != *it)) {
      return 0;
    } else {
      erase_from_bucket(bucket_index, it);
      return 1;
    }
  }

  iterator_type erase_from_bucket(size_t bucket_index, iterator_type iter) {
    auto &lo = buckets_[bucket_index << 1];
    if (iter == lo) {
      ++lo;
    }

    if (iter != begin()) {
      auto prev_bucket = bucket(*prev(iter));
      auto &prev_lo = buckets_[prev_bucket << 1];
      auto &prev_hi = buckets_[(prev_bucket << 1) + 1];
      if (iter == prev_hi) {
        ++prev_hi;
        if (iter == prev_lo) {
          ++prev_lo;
        }
      }
    }

    return values_.erase(iter);
  }

public:
  iterator_type find(const V &value) {
    return find_in_bucket(bucket(value), value);
  }

  const_iterator_type find(const V &value) const {
    return const_iterator_type(find_in_bucket(bucket(value), value));
  }

  [[nodiscard]] bool contains(const K &key) const noexcept {
    return find_key_in_bucket(bucket_from_key(key), key) != end();
  }

  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(const V &value) {
    auto [to_rehash, next_buckets_size] = need_rehash();
    if (to_rehash) {
      rehash(next_buckets_size);
    }

    // find insertion point
    auto bucket_index = bucket(value);
    auto &lo = buckets_[bucket_index << 1];
    auto &hi = buckets_[(bucket_index << 1) + 1];

    if (lo == hi) { // empty bucket
      values_.push_front(value);
      lo = begin();
      hi = next(lo);
      if constexpr (AllowDup) {
        return lo;
      } else {
        return {lo, true};
      }
    } else {
      auto it = ranges::lower_bound(lo, hi, value, less<K>{}, KeyProj{});
      if constexpr (!AllowDup) {
        if (it != hi && KeyProj{}(*it) == KeyProj{}(value)) {
          return {it, false};
        }
      }
      auto it_old = it;
      auto &prev_hi = (it_old != begin()) ? buckets_[(bucket(*prev(it_old)) << 1) + 1] : hi;
      it = values_.insert(it, value);
      if (lo == it_old) {
        lo = it;
      }
      if (prev_hi != hi && prev_hi == it_old) {
        prev_hi = it;
      }
      if constexpr (AllowDup) {
        return it;
      } else {
        return {it, true};
      }
    }
  }

  iterator_type erase(iterator_type iter) {
    if (iter == end()) {
      throw invalid_argument("attempt to erase end()");
    }
    return erase_from_bucket(bucket(*iter), iter);
  }

  size_t erase(const K &key) {
    auto bucket_index = bucket_from_key(key);
    if constexpr (AllowDup) {
      return erase_key_from_bucket(bucket_index, key);
    } else {
      // TODO: for HashMultiSet and HashMultiMap, optimize erase elements with
      // the same keys by implementing range erase...
      size_t erased = 0;
      while (erase_key_from_bucket(bucket_index, key)) {
        ++erased;
      }
      return erased;
    }
  }
};

} // namespace detail

template <Containable K, typename Hasher = Hash<K>>
using HashSet = detail::HashTable<K, K, Hasher, false>;

template <Containable K, typename Hasher = Hash<K>>
using HashMultiSet = detail::HashTable<K, K, Hasher, true>;

template <Containable K, Containable V, typename Hasher = Hash<K>>
using HashMap = detail::HashTable<K, pair<const K, V>, Hasher, false>;

template <Containable K, Containable V, typename Hasher = Hash<K>>
using HashMultiMap = detail::HashTable<K, pair<const K, V>, Hasher, true>;

} // namespace frozenca::hard

#endif //__CLRS4_HASH_TABLE_H__
