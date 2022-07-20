#ifndef __CLRS4_ALLOCATOR_FIXED__
#define __CLRS4_ALLOCATOR_FIXED__

#include <common.h>
#include <memory_resource>
#include <memorymappedfile.h>
#include <new>

namespace frozenca {

using namespace std;

template <Allocable T, size_t Count>
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
  void *do_allocate(size_t num_bytes, size_t alignment) override {
    assert(num_bytes == sizeof(Chunk) && alignment == alignof(Chunk));
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
    assert(num_bytes == sizeof(Chunk) && alignment == alignof(Chunk));
    auto x_chunk = reinterpret_cast<Chunk *>(x);
    x_chunk->next_ = free_;
    free_ = x_chunk;
  }

  bool do_is_equal(const pmr::memory_resource &other) const noexcept override {
    if (this == &other) {
      return true;
    }
    auto op = dynamic_cast<const MemoryResourceFixed *>(&other);
    return op && op->pool_ == pool_ && op->pool_size_ == pool_size_ &&
           op->free_ == free_;
  }
};

template <Allocable T, size_t Count = 1> class AllocatorFixed {
  pmr::memory_resource *mem_res_;

public:
  using value_type = T;

  explicit AllocatorFixed(
      pmr::memory_resource *mem_res = pmr::get_default_resource())
      : mem_res_{mem_res} {}

  T *allocate(size_t n) {
    return reinterpret_cast<T *>(
        mem_res_->allocate(sizeof(T) * n, align_val_t(alignof(T))));
  }

  void deallocate(T *ptr, size_t n) {
    mem_res->deallocate(reinterpret_cast<void *>(ptr), sizeof(T) * n);
  }
};

} // namespace frozenca

#endif //__CLRS4_ALLOCATOR_FIXED__
