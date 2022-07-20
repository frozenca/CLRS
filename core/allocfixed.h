#ifndef __CLRS4_ALLOC_FIXED__
#define __CLRS4_ALLOC_FIXED__

#include <common.h>
#include <memorymappedfile.h>

namespace frozenca {

using namespace std;

template <Allocable T, size_t Count = 1> class AllocatorFixed {
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
  using value_type = T;

  AllocatorFixed(unsigned char *pool_ptr, size_t pool_byte_size) {
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

  explicit AllocatorFixed(MemoryMappedFile &file)
      : AllocatorFixed(static_cast<unsigned char *>(file.data()), file.size()) {
  }

  T *allocate([[maybe_unused]] size_t n = Count) {
    assert(n == Count);
    if (free_ == pool_ + pool_size_) {
      throw invalid_argument("fixed allocator out of memory");
    } else {
      auto x = free_;
      free_ = x->next_;
      return reinterpret_cast<T *>(x);
    }
  }

  void deallocate(T *x, [[maybe_unused]] size_t n = Count) {
    assert(n == Count);
    auto x_chunk = reinterpret_cast<Chunk*>(x);
    x_chunk->next_ = free_;
    free_ = x_chunk;
  }
};

} // namespace frozenca

#endif //__CLRS4_ALLOC_FIXED__
