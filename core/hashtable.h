#ifndef __CLRS4_HASH_TABLE_H__
#define __CLRS4_HASH_TABLE_H__

#include <bit>
#include <common.h>
#include <hashfunction.h>
#include <iomanip>
#include <iostream>
#include <linkedlist.h>
#include <vector>

namespace frozenca {

using namespace std;

namespace detail {

template <Containable K, typename V, typename Hasher, bool AllowDup>
requires Hashable<Hasher, K>
class HashTable {
  // invariant: V is either K or pair<const K, Value> for some Value type.
  static constexpr bool is_set_ = is_same_v<K, V>;

  using Node = ListNode<V>;

  LinkedList<V> values_;

  // invariant: buckets_.size() == 0 or must be a power of 2
  // each "bucket" is a pair of two ListNode* [begin, before_end]
  // this is inclusive range to minimize invocation of Hasher{}().
  // invariant: for empty bucket, begin == before_end == nullptr
  // invariant: for nonempty bucket, begin <= before_end,
  //  and all node pointers between points to elements
  // with the same hash value, with keys in sorted order.
  using Buckets = vector<Node *>;
  Buckets buckets_;

  // invariant: buckets.size() == 0 or bucket_mask_ == buckets_.size() - 1
  size_t bucket_mask_ = 0;

  static constexpr size_t growth_factor_early_ = 4UL;
  static constexpr size_t growth_factor_ = 2UL;

  // ideally, this should be equal to half of the initial nonempty capacity of
  // std::vector...
  static constexpr size_t initial_nonempty_bucket_count_ = 8UL;
  static constexpr size_t bucket_count_threshold_ = 128UL;

  static constexpr float max_load_factor_ = 1.0f;

  const K& proj(const V& value) const noexcept {
    if constexpr (is_set_) {
      return value;
    } else {
      return value.first;
    }
  }

  // note that each invocation of Hasher{}(key) copies key every time!
  size_t bucket(const K& key) const noexcept {
    return Hasher{}(key) & bucket_mask_;
  }

  size_t bucket(const V& value) const noexcept requires (!is_set_) {
    return bucket(proj(value));
  }

public:
  using key_type = K;
  using value_type = V;
  using reference_type = V &;
  using const_reference_type = const V &;
  // invariant: K cannot be mutated
  // so if V is K, uses const iterator.
  // if V is pair<const K, value>, uses non-const iterator (but only value can be mutated)
  using iterator_type = ListIterator<V, is_set_>;
  using const_iterator_type = ListIterator<V, true>;
  // hash tables have no reverse iterators!

  HashTable() = default;
  ~HashTable() = default;
  HashTable(const HashTable &other) : values_{other.values_} {
    buckets_.clear();
    rehash(other.bucket_count());
  }
  HashTable &operator=(const HashTable &other) {
    values_ = other.values_;
    buckets_.clear();
    rehash(other.bucket_count());
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
        return {false, 0UL};
      }
    }
  }

  // find insertion point in inclusive range [lo, hi]
  // and whether there is already the same key
  pair<iterator_type, bool> find_insertion_point(Node *lo, Node *hi,
                                          const K &key) const noexcept {
    assert(lo && hi && hi != end().node_);
    auto curr = lo; // this is Node*, not iterator_type
    while (true) {
      if (proj(curr->key_) >= key) {
        return {iterator_type(curr), proj(curr->key_) == key};
      } else if (curr == hi) {
        return {iterator_type(hi->next_), false};
      }
      curr = curr->next_;
    }
  }

  // from first, find the first iterator which has the first different key
  // and that bucket index (to cache it)
  pair<iterator_type, size_t> find_next_key(iterator_type first) const noexcept {
    assert(first != end());
    auto iter = first;
    ++iter;
    while (iter != iterator_type(end())) {
      if (proj(*iter) != proj(*first)) {
        return {iter, bucket(*iter)};
      }
      ++iter;
    }
    return {end(), static_cast<size_t>(-1)};
  }

  void rehash(size_t next_bucket_size) {
    assert(has_single_bit(next_bucket_size));
    buckets_.assign(next_bucket_size << 1, nullptr);

    bucket_mask_ = next_bucket_size - 1;

    auto curr_it = begin();

    size_t cached_bucket_index = 0;
    bool cached = false;

    while (curr_it != end()) {
      auto bucket_index = cached ? cached_bucket_index : bucket(*curr_it);
      auto [next_it, next_bucket_index] = find_next_key(curr_it);
      cached = true;
      cached_bucket_index = next_bucket_index;

      auto &lo = buckets_[(bucket_index << 1)];
      auto &hi = buckets_[(bucket_index << 1) + 1];

      if (!lo) { // empty bucket
        assert(!hi);
        lo = curr_it.node_;
        hi = next_it.node_->prev_;
      } else { // find insertion point
        auto [where, unuse] = find_insertion_point(lo, hi, proj(*curr_it));
        bool to_prepend = (where.node_ == lo);
        bool to_append = (where.node_ == hi->next_);
        assert(where != next_it);
        if (where == curr_it) {
          assert(!to_prepend && to_append);
          // don't splice, just adjust hi
          hi = where.node_;
        } else {
          values_.splice(where, curr_it, next_it);
          if (to_prepend) {
            lo = curr_it.node_;
          }
          if (to_append) {
            hi = where.node_->prev_;
          }
        }
        
      }
      curr_it = next_it;
    }
  }

  const_iterator_type find_in_bucket(size_t bucket_index,
                               const K &key) const noexcept {
    auto lo = buckets_[bucket_index << 1];
    auto hi = buckets_[(bucket_index << 1) + 1];
    if (!lo) { // empty bucket
      return cend();
    }
    auto curr = lo;
    while (true) {
      if (proj(curr->key_) == key) {
        return const_iterator_type(curr);
      } else if (proj(curr->key_) > key || curr == hi) { // not found
        return cend();
      }
      curr = curr->next_;
    }
  }

  const_iterator_type find_in_bucket(size_t bucket_index,
                               const V& value) const noexcept requires (!is_set_) {
    auto lo = buckets_[bucket_index << 1];
    auto hi = buckets_[(bucket_index << 1) + 1];
    if (!lo) { // empty bucket
      return cend();
    }
    auto curr = lo;
    while (true) {
      if (*curr == value) {
        return const_iterator_type(curr);
      } else if (proj(curr->key_) > proj(value) || curr == hi) { // not found
        return cend();
      }
      curr = curr->next_;
    }
  }

  size_t erase_from_bucket(size_t bucket_index, const K &key) {
    auto &lo = buckets_[bucket_index << 1];
    auto &hi = buckets_[(bucket_index << 1) + 1];
    if (!lo) { // empty bucket
      return 0;
    }
    auto curr = lo;
    while (true) {
      if (proj(curr->key_) == key) { // found starting point
        break;
      } else if (proj(curr->key_) > key || curr == hi) { // not found
        return 0;
      }
      curr = curr->next_;
    }
    if (curr == hi) {
      auto first = curr;
      auto last = hi->next_;
      erase_from_bucket(bucket_index, first, last);
      return 1;
    }

    size_t count = 1;

    auto last = curr->next_;
    while (last != hi->next_ && proj(last->key_) == key) {
      last = last->next_;
      ++count;
    }
    erase_from_bucket(bucket_index, curr, last);
    return count;
  }

  iterator_type erase_from_bucket(size_t bucket_index, iterator_type iter) {
    return erase_from_bucket(bucket_index, iter, next(iter));
  }

  iterator_type erase_from_bucket(size_t bucket_index, iterator_type first,
                                  iterator_type last) {
    auto &lo = buckets_[bucket_index << 1];
    auto &hi = buckets_[(bucket_index << 1) + 1];
    assert(lo && hi);
    bool adjust_lo = (lo == first.node_);
    bool adjust_hi = (hi == last.node_->prev_);
    bool empty_bucket = adjust_lo && adjust_hi;
    if (empty_bucket) {
      lo = nullptr;
      hi = nullptr;
    } else if (adjust_lo) {
      lo = last.node_;
    } else if (adjust_hi) {
      hi = first.node_->prev_;
    }
    auto res = values_.erase(first, last);
    
    return res;
  }

public:
  iterator_type find(const V &value) {
    return iterator_type(find_in_bucket(bucket(value), value));
  }

  const_iterator_type find(const V &value) const {
    return const_iterator_type(find_in_bucket(bucket(value), value));
  }

  [[nodiscard]] bool contains(const K &key) const noexcept {
    return find_in_bucket(bucket(key), key) != end();
  }

private:
  template <typename T>
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert_value(T&& value) requires is_same_v<remove_cvref_t<T>, V> {
    auto [to_rehash, next_buckets_size] = need_rehash();
    if (to_rehash) {
      rehash(next_buckets_size);
    }

    // find insertion point
    auto bucket_index = bucket(value);
    auto &lo = buckets_[bucket_index << 1];
    auto &hi = buckets_[(bucket_index << 1) + 1];

    if (!lo) { // empty bucket
      assert(!hi);
      values_.push_front(forward<T>(value));
      lo = begin().node_;
      hi = lo;
      if constexpr (AllowDup) {
        return lo;
      } else {
        return {lo, true};
      }
    } else {
      assert(lo && hi);

      auto [where, exist] = find_insertion_point(lo, hi, proj(value));
      if constexpr (!AllowDup) {
        if (exist) {
          return {where, false};
        }
      }
      bool to_prepend = (where.node_ == lo);
      bool to_append = (where.node_ == hi->next_);

      auto it = values_.insert(where, forward<T>(value));

      if (to_prepend) {
        lo = it.node_;
      }
      if (to_append) {
        hi = where.node_->prev_;
      }

      if constexpr (AllowDup) {
        return it;
      } else {
        return {it, true};
      }
    }
  }

public:
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(const V& key) {
    return insert_value(key);
  }

  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(V&& key) {
    return insert_value(move(key));
  }

  template <typename T>
  auto& operator[](T&& raw_key) requires (!is_set_ && !AllowDup) {
    auto [to_rehash, next_buckets_size] = need_rehash();
    if (to_rehash) {
      rehash(next_buckets_size);
    }

    K key {forward<T>(raw_key)};

    // find insertion point
    auto bucket_index = bucket(key);
    auto &lo = buckets_[bucket_index << 1];
    auto &hi = buckets_[(bucket_index << 1) + 1];

    if (!lo) { // empty bucket
      assert(!hi);
      values_.push_front({move(key), {}});
      lo = begin().node_;
      hi = lo;
      return lo->key_.second;
    } else {
      assert(lo && hi);

      auto [where, exist] = find_insertion_point(lo, hi, key);
      if (exist) {
        return where->second;
      }
      bool to_prepend = (where.node_ == lo);
      bool to_append = (where.node_ == hi->next_);

      auto it = values_.insert(where, {move(key), {}});

      if (to_prepend) {
        lo = it.node_;
      }
      if (to_append) {
        hi = where.node_->prev_;
      }

      return it->second;
    }
  }

  iterator_type erase(iterator_type iter) {
    if (iter == end()) {
      throw invalid_argument("attempt to erase end()");
    }
    return erase_from_bucket(bucket(*iter), iter);
  }

  size_t erase(const K &key) { return erase_from_bucket(bucket(key), key); }
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

} // namespace frozenca

#endif //__CLRS4_HASH_TABLE_H__
