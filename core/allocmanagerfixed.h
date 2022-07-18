#ifndef __CLRS4_ALLOC_MANAGER_FIXED__
#define __CLRS4_ALLOC_MANAGER_FIXED__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

// deals with offsets only, not does actual allocations/deallocations
template <size_t PoolSizeBase = 16UL> class AllocManagerFixed {
  static_assert(PoolSizeBase > 0, "pool size too small");
  static_assert(PoolSizeBase <= 22UL, "pool size too big");

  struct Chunk {
    static constexpr unsigned char num_blocks_ = AllocManagerFixed::num_blocks_;

    Chunk() {
      unsigned char i = 0;
      for (unsigned char *p = &data_[0]; p != (&data_[0] + num_blocks_); ++p) {
        *p = ++i;
      }
    }

    unsigned char allocate() {
      unsigned char idx = first_available_;
      unsigned char *result = &data_[first_available_];
      first_available_ = *result;
      --blocks_available_;
      return idx;
    }

    void deallocate(unsigned char index) {
      auto *to_release = &data_[index];
      *to_release = first_available_;
      first_available_ = index;
      ++blocks_available_;
    }

    [[nodiscard]] bool has_available() const {
      return (blocks_available_ == num_blocks_);
    }

    [[nodiscard]] bool is_filled() const { return !blocks_available_; }

    unsigned char data_[num_blocks_];
    unsigned char first_available_ = 0;
    unsigned char blocks_available_ = num_blocks_;
  };

  static constexpr unsigned char num_blocks_ =
      numeric_limits<unsigned char>::max();
  vector<Chunk> chunks_;
  Chunk *alloc_chunk_ = nullptr;
  Chunk *empty_chunk_ = nullptr;

public:
  AllocManagerFixed() { chunks_.reserve((1UL << PoolSizeBase) - 1); }

  size_t allocate() {
    assert(!empty_chunk_ || empty_chunk_->has_available());
    if (!alloc_chunk_ || alloc_chunk_->is_filled()) {
      if (empty_chunk_) {
        alloc_chunk_ = empty_chunk_;
        empty_chunk_ = nullptr;
      } else {
        auto it = ranges::find_if(
            chunks_, [](const auto &chunk) { return !chunk.is_filled(); });
        if (it == chunks_.end()) {
          chunks_.emplace_back();
          alloc_chunk_ = &chunks_.back();
        } else {
          alloc_chunk_ = &*it;
        }
      }
    } else if (alloc_chunk_ == empty_chunk_) {
      empty_chunk_ = nullptr;
    }
    assert(alloc_chunk_ && !alloc_chunk_->is_filled());
    assert(!empty_chunk_ || empty_chunk_->has_available());
    size_t alloc_chunk_index =
        static_cast<size_t>(alloc_chunk_ - chunks_.data());
    assert(alloc_chunk_index < chunks_.size());
    size_t index_in_chunk = static_cast<size_t>(alloc_chunk_->allocate());
    return alloc_chunk_index * static_cast<size_t>(num_blocks_) +
           index_in_chunk;
  }

  void deallocate(size_t index) noexcept {
    assert(!chunks_.empty());
    assert(&chunks_.front() <= alloc_chunk_);
    assert(&chunks_.back() >= alloc_chunk_);

    Chunk *dealloc_chunk =
        chunks_.data() + (index / static_cast<size_t>(num_blocks_));
    assert((index / static_cast<size_t>(num_blocks_)) < chunks_.size());
    assert(!empty_chunk_ || empty_chunk_->has_available());

    // double free check
    assert(empty_chunk_ != dealloc_chunk);
    assert(!dealloc_chunk->has_available());
    assert(!empty_chunk_ || empty_chunk_->has_available());

    auto index_in_chunk =
        static_cast<unsigned char>(index % static_cast<size_t>(num_blocks_));
    dealloc_chunk->deallocate(index_in_chunk);

    if (dealloc_chunk->has_available()) {
      // only release chunk if there are 2 empty chunks.
      if (empty_chunk_) {
        // if last chunk is empty, just let dealloc_chunk points
        // to empty chunk, and release the last.
        // otherwise, swap two and release an empty chunk
        Chunk *last_chunk = &chunks_.back();
        if (last_chunk == dealloc_chunk) {
          dealloc_chunk = empty_chunk_;
        } else if (last_chunk != empty_chunk_) {
          swap(*empty_chunk_, *last_chunk);
        }
        assert(last_chunk->has_available());
        chunks_.pop_back();
      }
      empty_chunk_ = dealloc_chunk;
    }
  }
};

} // namespace frozenca

#endif //__CLRS4_ALLOC_MANAGER_FIXED__
