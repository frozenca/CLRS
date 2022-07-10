#ifndef __CLRS4_DYNAMIC_ARRAY_H__
#define __CLRS4_DYNAMIC_ARRAY_H__

#include <algorithm>
#include <bit>
#include <common.h>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace frozenca::hard {

using namespace std;

template <Containable T> class DynamicArray {
  ptrdiff_t size_ = 0;
  size_t capacity_ = 0;
  T *buf_ = nullptr;

public:
  using size_type = ptrdiff_t;
  using value_type = T;
  using reference_type = T &;
  using const_reference_type = const T &;
  using pointer_type = T *;
  using const_pointer_type = const T *;
  using iterator_type = T *;
  using const_iterator_type = const T *;
  using reverse_iterator_type = reverse_iterator<iterator_type>;
  using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

  DynamicArray() = default;

  ~DynamicArray() noexcept { allocator<T>{}.deallocate(buf_, capacity_); }

  DynamicArray(ptrdiff_t size)
      : size_{size}, capacity_{bit_ceil(static_cast<size_t>(size))} {
    buf_ = allocator<T>{}.allocate(capacity_);
  }

  DynamicArray(ptrdiff_t size, const T &value) : DynamicArray(size) {
    ranges::uninitialized_fill_n(buf_, size_, value);
  }

  DynamicArray(const DynamicArray &l) : DynamicArray(l.size_) {
    ranges::uninitialized_copy_n(buf_, size_, l.buf_, l.buf_ + l.size_);
  }

  DynamicArray &operator=(const DynamicArray &l) {
    DynamicArray arr(l);
    swap(*this, arr);
    return *this;
  }

  DynamicArray(DynamicArray &&l) noexcept
      : size_{l.size_}, capacity_{bit_ceil(static_cast<size_t>(l.size_))},
        buf_{l.buf_} {
    l.size_ = 0;
    l.capacity_ = 0;
    l.buf_ = nullptr;
  }

  DynamicArray &operator=(DynamicArray &&l) noexcept {
    DynamicArray arr(move(l));
    swap(*this, arr);
    return *this;
  }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  [[nodiscard]] ptrdiff_t size() const noexcept { return size_; }

  [[nodiscard]] ptrdiff_t capacity() const noexcept { return capacity_; }

  [[nodiscard]] iterator_type begin() noexcept { return buf_; }

  [[nodiscard]] const_iterator_type begin() const noexcept {
    return const_cast<const_pointer_type>(buf_);
  }

  [[nodiscard]] const_iterator_type cbegin() const noexcept {
    return const_cast<const_pointer_type>(buf_);
  }

  [[nodiscard]] iterator_type end() noexcept { return buf_ + size_; }

  [[nodiscard]] const_iterator_type end() const noexcept {
    return const_cast<const_pointer_type>(buf_ + size_);
  }

  [[nodiscard]] const_iterator_type cend() const noexcept {
    return const_cast<const_pointer_type>(buf_ + size_);
  }

  [[nodiscard]] reverse_iterator_type rbegin() noexcept {
    return reverse_iterator_type(end());
  }

  [[nodiscard]] const_reverse_iterator_type rbegin() const noexcept {
    return const_reverse_iterator_type(end());
  }

  [[nodiscard]] const_reverse_iterator_type crbegin() const noexcept {
    return const_reverse_iterator_type(end());
  }

  [[nodiscard]] reverse_iterator_type rend() noexcept {
    return reverse_iterator_type(begin());
  }

  [[nodiscard]] const_reverse_iterator_type rend() const noexcept {
    return const_reverse_iterator_type(begin());
  }

  [[nodiscard]] const_reverse_iterator_type crend() const noexcept {
    return const_reverse_iterator_type(begin());
  }

  reference_type operator[](size_type pos) { return *(buf_ + pos); }

  const_reference_type operator[](size_type pos) const {
    return *(const_cast<const_pointer_type>(buf_) + pos);
  }

  reference_type front() { return *(buf_); }

  const_reference_type front() const {
    return *(const_cast<const_pointer_type>(buf_));
  }

  reference_type back() { return *(buf_ + size_ - 1); }

  const_reference_type back() const {
    return *(const_cast<const_pointer_type>(buf_) + size_ - 1);
  }

private:
  void enlarge(size_type count) {
    assert(count > size_);
    auto new_capacity = bit_ceil(static_cast<size_t>(count));
    if (new_capacity > capacity_) {
      auto new_buf = allocator<T>{}.allocate(new_capacity);
      ranges::uninitialized_move_n(buf_, size_, new_buf, new_buf + size_);
      allocator<T>{}.deallocate(buf_, capacity_);
      capacity_ = new_capacity;
      buf_ = new_buf;
    }
  }

public:
  void resize(size_type count) {
    if (count < 0) {
      throw invalid_argument("negative size\n");
    }
    if (count < size_) {
      auto new_last = buf_ + count;
      auto to_destroy = size_ - count;
      ranges::destroy_n(new_last, to_destroy);
      size_ = count;
    } else if (count > size_) {
      enlarge(count);
      auto to_construct = count - size_;
      auto old_end = buf_ + size_;
      for (index_t i = 0; i < to_construct; ++i) {
        ranges::construct_at(old_end++);
      }
      size_ = count;
    }
  }

  void push_back(const T &value) {
    enlarge(size_ + 1);
    ranges::construct_at(buf_ + size_, value);
    size_++;
  }

  void push_back(T &&value) {
    enlarge(size_ + 1);
    ranges::construct_at(buf_ + size_, move(value));
    size_++;
  }

  template <typename... Args> reference_type emplace_back(Args &&...args) {
    enlarge(size_ + 1);
    ranges::construct_at(buf_ + size_, forward<Args>(args)...);
    auto ptr = buf_ + size_;
    size_++;
    return *ptr;
  }

  void pop_back() {
    ranges::destroy_at(buf_ + size_ - 1);
    size_--;
  }
};

} // namespace frozenca::hard

#endif //__CLRS4_DYNAMIC_ARRAY_H__
