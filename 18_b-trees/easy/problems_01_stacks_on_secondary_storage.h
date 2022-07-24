#ifndef __CLRS4_STACK_SECONDARY_STORAGE_H__
#define __CLRS4_STACK_SECONDARY_STORAGE_H__

#include <common.h>
#include <bit>
#include <disk_fixed_alloc.h>
#include <iostream>
#include <span>

namespace frozenca {

using namespace std;

class DiskStack {
  static constexpr size_t buf_size_ = 1 << 20;
  static constexpr size_t page_size_log_ = 12ul;
  
  MemoryMappedFile mm_file_;
  MemoryResourceFixed<index_t, buf_size_> mem_res_;
  AllocatorFixed<index_t, buf_size_> alloc_;
  span<index_t, buf_size_> data_;

  index_t curr_size_ = 0;

  index_t page_changes_ = 0;
  size_t curr_page_ = 0;

public:
  DiskStack(const filesystem::path &path, size_t pool_size, bool trunc = false)
      : mm_file_{path, pool_size, trunc},
        mem_res_{reinterpret_cast<unsigned char *>(mm_file_.data()),
                 mm_file_.size()}, alloc_(&mem_res_),
        data_(alloc_.allocate(buf_size_), buf_size_) {
            curr_page_ = bit_cast<size_t>(data_.data()) >> (page_size_log_);
        }

  ~DiskStack() {
    alloc_.deallocate(data_.data(), buf_size_);
  }

  void push(index_t val) {
    data_[curr_size_] = val;
    curr_size_++;
    auto new_page = bit_cast<size_t>(data_.data() + curr_size_) >> (page_size_log_);
    if (new_page != curr_page_) {
        page_changes_++;
        curr_page_ = new_page;
    }
  }

  [[nodiscard]] bool empty() const noexcept {
    return curr_size_ == 0;
  }

  index_t pop() {
    auto val = data_[curr_size_ - 1];
    curr_size_--;
    auto new_page = bit_cast<size_t>(data_.data() + curr_size_) >> (page_size_log_);
    if (new_page != curr_page_) {
        page_changes_++;
        curr_page_ = new_page;
    }
    return val;
  }

  [[nodiscard]] index_t page_changes() const noexcept {
    return page_changes_;
  }

};

} // namespace frozenca

#endif //__CLRS4_STACK_SECONDARY_STORAGE_H__
