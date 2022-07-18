#ifndef __CLRS4_FILE_MEMORY_RESOURCE__
#define __CLRS4_FILE_MEMORY_RESOURCE__

#include <common.h>
#include <fileallocator.h>
#include <filesystem>
#include <memory>
#include <memory_resource>
#include <stdexcept>

namespace frozenca {

using namespace std;

template <typename FileAlloc>
class FileMemoryResource : public pmr::memory_resource {
private:
  MemoryMappedFile file_;
  FileAlloc alloc_;

public:
  FileMemoryResource(const filesystem::path &path)
      : file_{path}, alloc_{file_} {}

private:
  void *do_allocate(size_t bytes, size_t alignment) override {
    return alloc_.allocate(bytes, alignment);
  }

  void do_deallocate(void *ptr, size_t bytes, size_t alignment) override {
    alloc_.deallocate(ptr, bytes, alignment);
  }

  bool do_is_equal(const pmr::memory_resource &other) const noexcept override {
    if (this == &other) {
      return true;
    }
    auto op = dynamic_cast<const FileMemoryResource *>(&other);
    return op && op->file_ == file_ && op->alloc_ == alloc_;
  }
};

class FileArenaResource : public pmr::memory_resource {
private:
  MemoryMappedFile file_;

public:
  FileArenaResource(const filesystem::path &path) : file_{path} {}

private:
  void *do_allocate(size_t bytes, size_t alignment) override {
    if (bytes % alignment) {
      throw invalid_argument("alloc bytes aren't aligned");
    }
    if (bytes > file_.size()) {
      file_.resize(bytes);
    }
    return file_.data();
  }

  void do_deallocate(void *ptr, size_t bytes, size_t alignment) override {
    if (bytes % alignment) {
      throw invalid_argument("dealloc bytes aren't aligned");
    }
    if (ptr != file_.data()) {
      throw invalid_argument(
          "dealloc arena pointer is not equal to begin of the file");
    }
    if (bytes != static_cast<size_t>(file_.size())) {
      throw invalid_argument("dealloc arena size is not equal to file size");
    }
  }

  bool do_is_equal(const pmr::memory_resource &other) const noexcept override {
    if (this == &other) {
      return true;
    }
    auto op = dynamic_cast<const FileArenaResource *>(&other);
    return op && op->file_ == file_;
  }
};

} // namespace frozenca

#endif //__CLRS4_MEMORY_MAPPED_FILE__
