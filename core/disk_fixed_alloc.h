#ifndef __FC_DISK_FIXED_ALLOC_H__
#define __FC_DISK_FIXED_ALLOC_H__

#include <bit>
#include <cassert>
#include <common.h>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <memory_resource>
#include <mmfile.h>
#include <stdexcept>
#include <type_traits>


namespace frozenca {

using namespace std;

template <DiskAllocable T, size_t Count>
class MemoryResourceFixed : public pmr::memory_resource {
  static_assert(Count > 0, "count should be nonzero");
  union Chunk {
    Chunk *next_;
    T object_[Count];
  };
  static_assert(!(sizeof(Chunk) % sizeof(T)) && !(sizeof(Chunk) % sizeof(T *)),
                "Chunk is not aligned");

  Chunk *pool_ = nullptr;
  size_t pool_size_ = 0;
  Chunk *free_ = nullptr;

public:
  MemoryResourceFixed(unsigned char *pool_ptr, size_t pool_byte_size) {
    if (!pool_ptr) {
      throw invalid_argument("pool ptr is null");
    }
    if ((bit_cast<size_t>(pool_ptr) % sizeof(T)) ||
        (bit_cast<size_t>(pool_ptr) % sizeof(T *))) {
      throw invalid_argument("pool ptr is not aligned with T/T*");
    }
    if (pool_byte_size < sizeof(Chunk)) {
      throw invalid_argument("pool byte size is too small");
    }
    if ((pool_byte_size % sizeof(T)) || (pool_byte_size % sizeof(T *))) {
      throw invalid_argument("pool byte size is not aligned with T/T*");
    }

    pool_ = reinterpret_cast<Chunk *>(pool_ptr);
    // size in chunks
    pool_size_ = pool_byte_size / sizeof(Chunk);

    auto curr_chunk = pool_;
    for (size_t i = 0; i < pool_size_; i++, curr_chunk++) {
      curr_chunk->next_ = curr_chunk + 1;
    }
    free_ = pool_;
  }

  explicit MemoryResourceFixed(MemoryMappedFile &file)
      : MemoryResourceFixed(static_cast<unsigned char *>(file.data()),
                            file.size()) {}

private:
  void *do_allocate([[maybe_unused]] size_t num_bytes,
                    [[maybe_unused]] size_t alignment) override {
    assert((num_bytes % sizeof(Chunk)) == 0 &&
           (alignment % alignof(Chunk)) == 0);
    if (free_ == pool_ + pool_size_) {
      throw invalid_argument("fixed allocator out of memory");
    } else {
      auto x = free_;
      free_ = x->next_;
      return reinterpret_cast<void *>(x);
    }
  }

  void do_deallocate(void *x, [[maybe_unused]] size_t num_bytes,
                     [[maybe_unused]] size_t alignment) override {
    assert((num_bytes % sizeof(Chunk) == 0) &&
           (alignment % alignof(Chunk)) == 0);
    auto x_chunk = reinterpret_cast<Chunk *>(x);
    x_chunk->next_ = free_;
    free_ = x_chunk;
  }

  [[nodiscard]] bool
  do_is_equal(const pmr::memory_resource &other) const noexcept override {
    if (this == &other) {
      return true;
    }
    auto op = dynamic_cast<const MemoryResourceFixed *>(&other);
    return op && op->pool_ == pool_ && op->pool_size_ == pool_size_ &&
           op->free_ == free_;
  }
};

template <DiskAllocable T, size_t Count = 1> class AllocatorFixed {
  pmr::memory_resource *mem_res_;

public:
  template <typename Other> struct rebind {
    using other = AllocatorFixed<Other, Count>;
  };

  using value_type = T;

  explicit AllocatorFixed(
      pmr::memory_resource *mem_res = pmr::get_default_resource())
      : mem_res_{mem_res} {}

  template <typename Other, size_t CountOther>
  AllocatorFixed(const AllocatorFixed<Other, CountOther> &other)
      : AllocatorFixed(other.get_memory_resource()) {}

  T *allocate(size_t n) {
    return reinterpret_cast<T *>(
        mem_res_->allocate(sizeof(T) * n, alignment_of_v<T>));
  }

  void deallocate(T *ptr, size_t n) {
    mem_res_->deallocate(reinterpret_cast<void *>(ptr), sizeof(T) * n);
  }

  [[nodiscard]] pmr::memory_resource *get_memory_resource() const noexcept {
    return mem_res_;
  }
};

template <typename> struct isDiskAlloc : false_type {};

template <DiskAllocable T, size_t Count>
struct isDiskAlloc<AllocatorFixed<T, Count>> : true_type {};

} // namespace frozenca

#endif //__FC_DISK_FIXED_ALLOC_H__
