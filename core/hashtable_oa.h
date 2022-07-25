#ifndef __CLRS4_HASH_TABLE_OPEN_ADDRESS_H__
#define __CLRS4_HASH_TABLE_OPEN_ADDRESS_H__

#include <bit>
#include <common.h>
#include <hashfunction.h>
#include <iomanip>
#include <iostream>
#include <vector>

namespace frozenca {

using namespace std;

namespace detail {

template <Containable K, typename V, typename Hasher, bool AllowDup>
requires Hashable<Hasher, K>
class HashTableOpenAddress {

  using key_type = K;
  using value_type = pair<V, int>;
  using reference_type = value_type&;
  using const_reference_type = const value_type&;

  template <bool Const> class HashTableOAIterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = HashTableOpenAddress::value_type;
    using pointer = conditional_t<Const, const value_type*, value_type*>;
    using reference = conditional_t<Const, const value_type&, value_type&>;
    using iterator_category = bidirectional_iterator_tag;
    using iterator_concept = iterator_category;

  public:
    pointer ptr_ = nullptr;

    HashTableOAIterator() = default;
    HashTableOAIterator(pointer ptr) : ptr_{ptr} {}

    HashTableOAIterator(const HashTableOAIterator<!Const> &other)
        : ptr_{other.ptr_} {}

    HashTableOAIterator(const HashTableOAIterator &other) = default;
    HashTableOAIterator &operator=(const HashTableOAIterator &other) = default;
    HashTableOAIterator(HashTableOAIterator &&other) noexcept = default;
    HashTableOAIterator &
    operator=(HashTableOAIterator &&other) noexcept = default;

    reference operator*() const noexcept { return *ptr_; }

    pointer operator->() const noexcept { return ptr_; }

    HashTableOAIterator &operator++() {
      auto index = ptr_ - values_.data();
      if (index < ssize(values_)) {
        ++index;
        ++ptr_;
      }
      while (index < ssize(values_) && !values_[index].second) {
        ++index;
        ++ptr_;
      }
      return *this;
    }

    HashTableOAIterator operator++(int) {
      HashTableOAIterator temp = *this;
      auto index = ptr_ - values_.data();
      if (index < ssize(values_)) {
        ++index;
        ++ptr_;
      }
      while (index < ssize(values_) && !values_[index].second) {
        ++index;
        ++ptr_;
      }
      return temp;
    }

    HashTableOAIterator &operator--() {
      auto index = ptr_ - values_.data();
      if (index > 0) {
        --index;
      }
      while (index > 0 && !values_[index].second) {
        --index;
      }
      return *this;
    }

    HashTableOAIterator operator--(int) {
      HashTableOAIterator temp = *this;
      auto index = ptr_ - values_.data();
      if (index > 0) {
        --index;
      }
      while (index > 0 && !values_[index].second) {
        --index;
      }
      return temp;
    }

    [[nodiscard]] friend bool operator==(const HashTableOAIterator &lhs,
                                         const HashTableOAIterator &rhs) {
      return lhs.ptr_ == rhs.ptr_;
    }
  };

  // invariant: V is either K or pair<const K, Value> for some Value type.
  static constexpr bool is_set_ = is_same_v<K, V>;

  vector<value_type> values_;

  ptrdiff_t num_entries_ = 0;

  static constexpr size_t growth_factor_early_ = 4UL;
  static constexpr size_t growth_factor_ = 2UL;

  // ideally, this should be equal to half of the initial nonempty capacity of
  // std::vector...
  static constexpr size_t initial_nonempty_bucket_count_ = 8UL;
  static constexpr size_t bucket_count_threshold_ = 128UL;

  static constexpr float max_load_factor_ = 0.6666f;

  const K &proj(const V &value) const noexcept {
    if constexpr (is_set_) {
      return value;
    } else {
      return value.first;
    }
  }

  // note that each invocation of Hasher{}(key) copies key every time!
  size_t bucket(const K &key) const noexcept {
    return Hasher{}(key) % values_.size();
  }

  size_t bucket(const V &value) const noexcept requires(!is_set_) {
    return bucket(proj(value));
  }

public:
  // invariant: K cannot be mutated
  // so if V is K, uses const iterator.
  // if V is pair<const K, value>, uses non-const iterator (but only value can
  // be mutated)

  using iterator_type = HashTableOAIterator<is_set_>;
  using const_iterator_type = HashTableOAIterator<true>;
  // hash tables have no reverse iterators!

  HashTableOpenAddress() = default;
  ~HashTableOpenAddress() = default;
  HashTableOpenAddress(const HashTableOpenAddress &other)
      : values_{other.values_} {
    rehash(other.bucket_count());
  }
  HashTableOpenAddress &operator=(const HashTableOpenAddress &other) {
    values_ = other.values_;
    rehash(other.bucket_count());
    return *this;
  }
  HashTableOpenAddress(HashTableOpenAddress &&other) noexcept = default;
  HashTableOpenAddress &
  operator=(HashTableOpenAddress &&other) noexcept = default;

  [[nodiscard]] iterator_type begin() noexcept {
    return iterator_type(values_.data());
  }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_iterator_type(values_.data());
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_iterator_type(values_.data());
  }

  [[nodiscard]] iterator_type end() noexcept {
    return iterator_type(values_.data() + values_.size());
  }

  [[nodiscard]] const_iterator_type end() const noexcept {
    return const_iterator_type(values_.data() + values_.size());
  }

  [[nodiscard]] const_iterator_type cend() const noexcept {
    return const_iterator_type(values_.data() + values_.size());
  }

  [[nodiscard]] bool empty() const noexcept { return num_entries_ == 0; }

  [[nodiscard]] ptrdiff_t size() const noexcept { return num_entries_; }

  void clear() noexcept {
    values_.clear();
    num_entries_ = 0;
  }

  size_t bucket_count() const noexcept { return values_.size(); }

  float load_factor() const noexcept {
    if (empty()) {
      return 0.0f;
    }
    return static_cast<float>(size()) / static_cast<float>(values_.size());
  }

  float max_load_factor() const noexcept { return max_load_factor_; }

private:
  [[nodiscard]] pair<bool, size_t> need_rehash() const noexcept {
    if (empty()) {
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

  pair<size_t, bool> find_insertion_point(const K &key) const noexcept {
    auto q = bucket(key);
    bool turned = false;
    for (size_t i = 0; i < values_.size(); ++i) {
      if (!values_[q].second) {
        return {q, false};
      }
      if constexpr (!AllowDup) {
        if (proj(values_[q].first) == key) {
          return {q, true};
        }
      }
      q++;
      if (!turned && q >= values_.size()) {
        q -= values_.size();
        turned = true;
      }
    }
    return {values_.size(), true};
  }

  size_t find_key(const K &key) const noexcept {
    auto q = bucket(key);
    bool turned = false;
    for (size_t i = 0; i < values_.size(); ++i) {
      if (!values_[q].second) {
        return values_.size();
      } else if (proj(values_[q].first) == key) {
        return q;
      }
      q++;
      if (!turned && q >= values_.size()) {
        q -= values_.size();
        turned = true;
      }
    }
    return values_.size();
  }

  size_t find_value(const V &value) const noexcept {
    auto q = bucket(value);
    bool turned = false;
    for (size_t i = 0; i < values_.size(); ++i) {
      if (!values_[q].second) {
        return values_.size();
      } else if (values_[q].first == value) {
        return q;
      }
      q++;
      if (!turned && q >= values_.size()) {
        q -= values_.size();
        turned = true;
      }
    }
    return values_.size();
  }

  void insert_value_unchecked(V &&value) noexcept {
    auto q = bucket(value);
    bool turned = false;
    for (size_t i = 0; i < values_.size(); ++i) {
      if (!values_[q].second) {
        values_[q].first = move(value);
        values_[q].second = true;
        ++num_entries_;
        return;
      }
      q++;
      if (!turned && q >= values_.size()) {
        q -= values_.size();
        turned = true;
      }
    }
  }

  void rehash(size_t next_bucket_size) {
    assert(has_single_bit(next_bucket_size));
    vector<value_type> temp_values(next_bucket_size);
    swap(temp_values, values_);
    num_entries_ = 0;
    for (auto&& [old_value, was_filled] : temp_values) {
      if (was_filled) {
        insert_value_unchecked(move(old_value));
      }
    }
  }

  size_t erase_key(const K &key) {
    size_t count = 0;
    auto q = find_key(key);
    while (q != values_.size()) {
      erase_index(q);
      ++count;
      q = find_key(key);
    }
    return count;
  }

  void erase_index(size_t q) {
    --num_entries_;
    while (true) {
      values_[q].first = {};
      values_[q].second = false;
      auto q_i = q;
      while (true) {
        q_i = (q_i + 1) % values_.size();
        if (!values_[q_i].second) {
          return;
        }
        auto offset = values_.size() - bucket(values_[q_i].first);
        if ((q + offset) % values_.size() < (q_i + offset) % values_.size()) {
          break;
        }
      }
      values_[q] = move(values_[q_i]);
      q = q_i;
    }
  }

public:
  iterator_type find(const V &value) {
    return iterator_type(values_.data() + find_value(value));
  }

  const_iterator_type find(const V &value) const {
    return const_iterator_type(values_.data() + find_value(value));
  }

  [[nodiscard]] bool contains(const K &key) const noexcept {
    return find_key(key) != values_.size();
  }

private:
  template <typename T>
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert_value(T &&value) requires is_same_v<remove_cvref_t<T>, V> {
    auto [to_rehash, next_buckets_size] = need_rehash();
    if (to_rehash) {
      rehash(next_buckets_size);
    }

    auto [q, exist] = find_insertion_point(proj(value));
    auto it = iterator_type(values_.data() + q);
    if constexpr (!AllowDup) {
      if (exist) {
        return {it, false};
      }
    }

    values_[q].first = forward<T>(value);
    values_[q].second = true;
    ++num_entries_;

    if constexpr (AllowDup) {
      return it;
    } else {
      return {it, true};
    }
  }

public:
  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(const V &key) {
    return insert_value(key);
  }

  conditional_t<AllowDup, iterator_type, pair<iterator_type, bool>>
  insert(V &&key) {
    return insert_value(move(key));
  }

  template <typename T>
  auto &operator[](T &&raw_key) requires(!is_set_ && !AllowDup) {
    auto [to_rehash, next_buckets_size] = need_rehash();
    if (to_rehash) {
      rehash(next_buckets_size);
    }

    K key{forward<T>(raw_key)};

    auto [q, exist] = find_insertion_point(key);
    auto it = iterator_type(values_.data() + q);
    if (exist) {
      return values_[q].second;
    }
    values_[q].first = {move(key), {}};
    values_[q].second = true;
    ++num_entries_;
    return values_[q].second;
  }

  iterator_type erase(iterator_type iter) {
    if (iter == end()) {
      throw invalid_argument("attempt to erase end()");
    }
    auto q = &(*iter) - values_.data();
    ++iter;
    erase_index(q);
    return iter;
  }

  size_t erase(const K &key) { return erase_key(key); }
};

} // namespace detail

template <Containable K, typename Hasher = Hash<K>>
using HashSetOA = detail::HashTableOpenAddress<K, K, Hasher, false>;

template <Containable K, typename Hasher = Hash<K>>
using HashMultiSetOA = detail::HashTableOpenAddress<K, K, Hasher, true>;

template <Containable K, Containable V, typename Hasher = Hash<K>>
using HashMapOA =
    detail::HashTableOpenAddress<K, pair<const K, V>, Hasher, false>;

template <Containable K, Containable V, typename Hasher = Hash<K>>
using HashMultiMapOA =
    detail::HashTableOpenAddress<K, pair<const K, V>, Hasher, true>;

} // namespace frozenca

#endif //__CLRS4_HASH_TABLE_OPEN_ADDRESS_H__
