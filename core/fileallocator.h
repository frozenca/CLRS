#ifndef __CLRS4_FILE_ALLOCATOR__
#define __CLRS4_FILE_ALLOCATOR__

#include <allocmanagerfixed.h>
#include <cassert>
#include <common.h>
#include <limits>
#include <memory>
#include <memorymappedfile.h>

namespace frozenca {

using namespace std;

template <typename T>
concept Allocable = is_same_v<remove_cvref_t<T>, T> &&
    is_trivially_copyable_v<T> &&(sizeof(T) % alignof(T) == 0);

template <Allocable T, size_t count = 1> class FileFixedAllocator {
private:
  MemoryMappedFile &file_;
  AllocManagerFixed<16> manager_;

public:
  static_assert(count, "alloc count should be positive");
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using void_pointer = void *;
  using const_void_pointer = const void *;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  static constexpr size_t alloc_size_ = sizeof(T) * count;

  explicit FileFixedAllocator(MemoryMappedFile &file) : file_{file} {}

  pointer allocate([[maybe_unused]] size_type n = count) {
    assert(n == count);
    size_t offset = manager_.allocate();
    size_t offset_bytes = alloc_size_ * offset;
    if (offset_bytes >= file_.size()) {
      file_.resize(offset_bytes + alloc_size_);
    }
    return reinterpret_cast<pointer>(
        static_cast<unsigned char *>(file_.data()) + offset_bytes);
  }

  void deallocate(pointer p, [[maybe_unused]] size_type n = count) {
    assert(n == count);
    assert(reinterpret_cast<unsigned char *>(file_.data()) <=
               reinterpret_cast<unsigned char *>(p) &&
           reinterpret_cast<unsigned char *>(p) <
               reinterpret_cast<unsigned char *>(file_.data()) + file_.size());
    size_t offset_bytes = reinterpret_cast<unsigned char *>(p) -
                          reinterpret_cast<unsigned char *>(file_.data());
    assert((offset_bytes % alloc_size_) == 0);
    size_t index = offset_bytes / alloc_size_;
    manager_.deallocate(index);
  }

  template <typename... Args> void construct(pointer p, Args &&...args) {
    construct_at(p, forward<Args>(args)...);
  }

  void destroy(pointer p) { destroy_at(p); }
};

} // namespace frozenca

#endif //__CLRS4_FILE_ALLOCATOR__
