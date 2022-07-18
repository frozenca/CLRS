#ifndef __CLRS4_ALLOC_MANAGER_DEFAULT__
#define __CLRS4_ALLOC_MANAGER_DEFAULT__

#include <algorithm>
#include <bit>
#include <common.h>
#include <iostream>
#include <memory_resource>
#include <ranges>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

// sizeof(Chunk) == 32 in typical 64bit machines,
// but the 'foot' of the current chunk is actually
// represented as the prev_size of the (unallocated) next chunk
// foot - head it is at least and aligned as sizeof(Chunk)
// sizeof(Chunk) should be a power of 2

// chunks are look like this:
//
// chunk->     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//             |             Size of previous chunk, if unallocated (U clear)  |
// 	           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//             |             Size of chunk, in bytes                     |0|U|I|
//             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//             | Forward pointer to next chunk in list (only used if free)     |
//    	       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  	         | Back pointer to previous chunk in list (only used if free) |
//     	       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 	           |             Unused space (multiple of sizeof(Chunk) bytes)
// | nextchunk->
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//     `foot:' |             Size of chunk, in bytes (written by prev chunk)   |
// 	           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//        	   |             Size of next chunk, in bytes |0|U|I|
// 	           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

struct Chunk {
  size_t prev_size_ = 0;
  // invariant: size_ > 0
  // invariant: rightmost three (or two) bits of size_ are flags (CHUNK_IN_USE,
  // CHUNK_UNSORTED)
  size_t size_ = 0;
  // pointers to prev/next within bins, only used for free chunks.
  // nothing to do with prev/next chunks within the global arena, beware!
  Chunk *next_ = nullptr;
  Chunk *prev_ = nullptr;
};

static_assert(sizeof(size_t) == 8 || sizeof(size_t) == 4);
constexpr size_t CHUNK_IN_USE = 0x1;
constexpr size_t CHUNK_UNSORTED = 0x2;
constexpr size_t CHUNK_FLAG_MASK = 0x7;
constexpr size_t CHUNK_FLAG_UNMASK =
    (sizeof(size_t) == 8) ? 0xFFFFFFFFFFFFFFF8 : 0xFFFFFFF8;

struct GetSize {
  size_t operator()(Chunk *iter) const noexcept {
    assert(iter);
    return iter->size_;
  }
};

class AllocManagerDefault {
private:
  static constexpr size_t chunk_alignment_ = sizeof(Chunk);
  static_assert(has_single_bit(chunk_alignment_));
  static constexpr size_t align_multiplier = (sizeof(size_t) == 8) ? 1UL : 2UL;
  static constexpr size_t bin_shift_ = (sizeof(size_t) == 8) ? 5UL : 4UL;
  static constexpr size_t num_fast_bins_ = 8UL * align_multiplier;
  static constexpr size_t num_small_bins_ = 56UL * align_multiplier;

  static constexpr size_t min_fast_size_ = chunk_alignment_;
  static constexpr size_t min_small_size_ = chunk_alignment_ * num_fast_bins_;
  static constexpr size_t min_large_size_ =
      chunk_alignment_ * (num_small_bins_ + num_fast_bins_);

  size_t curr_pool_size_ = 0;

  // bins that store chunks of 32, 64, 96, ..., 224 bytes respectively
  // (or 16, 32, ..., 112)
  // LIFO singly linked list, NOT merged when freed
  vector<Chunk *> fast_bins_;

  // bins that store chunks of 256, 288, ..., 2016 bytes respectively
  // (or 128, 144, ..., 1008)
  // FIFO doubly linked list, chunks are merged with adjacent chunks when freed
  // if adjacent
  vector<Chunk *> small_bins_;

  // a *single* bin that store chunks of 2048+ bytes (or 1024+)
  // maintain the sorted order w.r.t. chunk size in *increasing* order
  // chunks are merged with adjacent chunks when freed if adjacent
  vector<Chunk *> large_bin_;

  // a bin for freed chunks and remainder chunks
  // when an allocation request fails to search chunk in other bins,
  // this bin is searched
  Chunk *unsorted_bin_ = nullptr;

  Chunk *top_chunk_ = nullptr;

  // store *all* chunks
  pmr::vector<unsigned char> all_chunks_;

public:
  AllocManagerDefault(size_t init_pool_size, pmr::memory_resource *mem_res =
                                                 pmr::get_default_resource())
      : curr_pool_size_{init_pool_size}, fast_bins_(num_fast_bins_, nullptr),
        small_bins_(num_small_bins_, nullptr),
        all_chunks_(init_pool_size,
                    pmr::polymorphic_allocator<unsigned char>(mem_res)) {
    assert(init_pool_size % chunk_alignment_ == 0);
    top_chunk_ = first_chunk();
    set_size(top_chunk_, init_pool_size);
  }

private:
  [[nodiscard]] bool is_begin(const Chunk *chunk) const noexcept {
    assert(chunk);
    assert(reinterpret_cast<const unsigned char *>(chunk) >=
           all_chunks_.data());
    return reinterpret_cast<const unsigned char *>(chunk) == all_chunks_.data();
  }

  Chunk *first_chunk() noexcept {
    return reinterpret_cast<Chunk *>(all_chunks_.data());
  }

  const Chunk *first_chunk() const noexcept {
    return reinterpret_cast<const Chunk *>(all_chunks_.data());
  }

  // don't need is_last, as top_chunk_ is always the last chunk

  size_t *get_foot(Chunk *chunk) noexcept {
    assert(chunk && chunk != top_chunk_);
    return reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(chunk) +
                                      realsize(chunk));
  }

  const size_t *get_foot(const Chunk *chunk) const noexcept {
    assert(chunk && chunk != top_chunk_);
    return reinterpret_cast<const size_t *>(
        reinterpret_cast<const unsigned char *>(chunk) + realsize(chunk));
  }

  [[nodiscard]] size_t get_flags(const Chunk *chunk) const noexcept {
    assert(chunk);
    return chunk->size_ & CHUNK_FLAG_MASK;
  }

  void set_flags(Chunk *chunk, size_t flags) {
    assert(chunk && chunk != top_chunk_);
    chunk->size_ &= CHUNK_FLAG_UNMASK;
    chunk->size_ |= flags;
    // add the foot
    size_t *foot = get_foot(chunk);
    *foot &= CHUNK_FLAG_UNMASK;
    *foot |= flags;
  }

  // erase flags and make a nonzero mulitple of chunk_alignment_
  size_t realsize(size_t sz) const noexcept {
    assert(sz >= chunk_alignment_);
    return (sz >> bin_shift_) << bin_shift_;
  }

  size_t realsize(const Chunk *chunk) const noexcept {
    assert(chunk && chunk->size_ >= chunk_alignment_);
    return realsize(chunk->size_);
  }

  void set_size(Chunk *chunk, size_t sz) {
    assert(chunk);
    chunk->size_ = sz;
    // add the foot
    if (chunk != top_chunk_) {
      size_t *foot = get_foot(chunk);
      *foot = sz;
    }
  }

  size_t verify_size(const Chunk *chunk) const noexcept {
    assert(chunk && chunk->size_ >= chunk_alignment_);
    // check the foot
    assert(chunk == top_chunk_ || *get_foot(chunk) == chunk->size_);
    return chunk->size_;
  }

  Chunk *prev_chunk(Chunk *chunk) noexcept {
    if (!chunk || is_begin(chunk)) {
      return nullptr;
    }
    return reinterpret_cast<Chunk *>(reinterpret_cast<unsigned char *>(chunk) -
                                     realsize(chunk->prev_size_));
  }

  const Chunk *prev_chunk(const Chunk *chunk) const noexcept {
    if (!chunk || is_begin(chunk)) {
      return nullptr;
    }
    return reinterpret_cast<const Chunk *>(
        reinterpret_cast<const unsigned char *>(chunk) -
        realsize(chunk->prev_size_));
  }

  Chunk *next_chunk(Chunk *chunk) noexcept {
    if (!chunk || chunk == top_chunk_) {
      return nullptr;
    }
    return reinterpret_cast<Chunk *>(reinterpret_cast<unsigned char *>(chunk) +
                                     realsize(chunk));
  }

  const Chunk *next_chunk(const Chunk *chunk) const noexcept {
    if (!chunk || chunk == top_chunk_) {
      return nullptr;
    }
    return reinterpret_cast<const Chunk *>(
        reinterpret_cast<const unsigned char *>(chunk) + realsize(chunk));
  }

  bool verify_fastbins() const {
    assert(fast_bins_.size() == num_fast_bins_);
    for (size_t i = 0; i < num_fast_bins_; ++i) {
      auto chunk = fast_bins_[i];
      while (chunk) {
        assert(realsize(verify_size(chunk)) == fast_size(i));
        assert(get_flags(chunk) == CHUNK_IN_USE);
        chunk = chunk->next_;
      }
    }
    return true;
  }

  bool verify_smallbins() const {
    assert(small_bins_.size() == num_small_bins_);
    for (size_t i = 0; i < num_small_bins_; ++i) {
      auto chunk = small_bins_[i];
      while (chunk) {
        assert(verify_size(chunk) == small_size(i));
        assert(chunk->prev_ && chunk->next_ && chunk->prev_->next_ == chunk &&
               chunk->next_->prev_ == chunk);
        if (chunk == chunk->next_) {
          assert(chunk == chunk->prev_);
          break;
        }
        chunk = chunk->next_;
        if (chunk == small_bins_[i]) {
          break;
        }
      }
    }
    return true;
  }

  bool verify_largebin() const {
    assert(ranges::is_sorted(large_bin_, ranges::less{}, GetSize{}));
    assert(ranges::all_of(large_bin_, [this](const auto &chunk) {
      return chunk && get_flags(chunk) == 0 &&
             verify_size(chunk) >= min_large_size_ &&
             (chunk->size_ % chunk_alignment_) == 0;
    }));
    return true;
  }

  bool verify_unsorted() const {
    auto chunk = unsorted_bin_;
    while (chunk) {
      assert(verify_size(chunk) >= chunk_alignment_ &&
             (realsize(chunk) % chunk_alignment_) == 0);
      assert(get_flags(chunk) == CHUNK_UNSORTED);
      assert(chunk->prev_ && chunk->next_ && chunk->prev_->next_ == chunk &&
             chunk->next_->prev_ == chunk);
      if (chunk == chunk->next_) {
        assert(chunk == chunk->prev_);
        break;
      }
      chunk = chunk->next_;
      if (chunk == unsorted_bin_) {
        break;
      }
    }
    return true;
  }

  bool verify() const {
    assert(!all_chunks_.empty() && all_chunks_.size() == curr_pool_size_);
    const Chunk *chunk = first_chunk();
    while (chunk) {
      assert(chunk->size_ >= chunk_alignment_ &&
             (realsize(chunk) % chunk_alignment_) == 0);
      auto next = next_chunk(chunk);
      if (!next) {
        assert(chunk == top_chunk_ && get_flags(chunk) == 0);
        assert(reinterpret_cast<const unsigned char *>(chunk) + chunk->size_ ==
               all_chunks_.data() + all_chunks_.size());
      }
      chunk = next;
    }
    assert(verify_fastbins());
    assert(verify_smallbins());
    assert(verify_largebin());
    assert(verify_unsorted());
    return true;
  }

  [[nodiscard]] size_t fast_index(size_t num_bytes) const noexcept {
    return (num_bytes >> bin_shift_);
  }

  [[nodiscard]] size_t fast_size(size_t idx) const noexcept {
    return (idx << bin_shift_);
  }

  [[nodiscard]] size_t small_index(size_t num_bytes) const noexcept {
    assert(num_bytes >= min_small_size_);
    return ((num_bytes - min_small_size_) >> bin_shift_);
  }

  [[nodiscard]] size_t small_size(size_t idx) const noexcept {
    return (idx << bin_shift_) + min_small_size_;
  }

  // where to insert in the large bin
  [[nodiscard]] auto large_iter(size_t num_bytes) const noexcept {
    return ranges::lower_bound(large_bin_, num_bytes, ranges::less{},
                               GetSize{});
  }

  void unlink_from_bin(Chunk *chunk, Chunk *&bin) {
    assert(chunk && chunk->prev_ && chunk->next_ && bin);
    if (chunk == chunk->prev_) {
      // make bin empty
      assert(chunk == chunk->next_);
      bin = nullptr;
    } else {
      if (chunk == bin) {
        bin = chunk->next_;
      }
      chunk->next_->prev_ = chunk->prev_;
      chunk->prev_->next_ = chunk->next_;
    }
    chunk->prev_ = nullptr;
    chunk->next_ = nullptr;
  }

  void push_front_to_bin(Chunk *chunk, Chunk *&bin) {
    push_back_to_bin(chunk, bin);
    bin = chunk;
  }

  void push_back_to_bin(Chunk *chunk, Chunk *&bin) {
    assert(chunk);
    if (!bin) {
      bin = chunk;
      chunk->prev_ = chunk;
      chunk->next_ = chunk;
    } else {
      chunk->next_ = bin;
      chunk->prev_ = bin->prev_;
      bin->prev_->next_ = chunk;
      bin->prev_ = chunk;
    }
  }

  Chunk *pop_back_from_bin(Chunk *&bin) {
    auto chunk = bin;
    if (chunk) {
      assert(chunk->prev_ && chunk->next_);
      chunk = chunk->prev_;
      unlink_from_bin(chunk, bin);
    }
    return chunk;
  }

  Chunk *pop_front_from_bin(Chunk *&bin) {
    auto chunk = bin;
    if (chunk) {
      assert(chunk->prev_ && chunk->next_);
      unlink_from_bin(chunk, bin);
    }
    return chunk;
  }

  Chunk *pop_fastbin(Chunk *&bin) {
    auto chunk = bin;
    if (chunk) {
      bin = bin->next_;
      chunk->next_ = nullptr;
      assert(!chunk->prev_);
    }
    return chunk;
  }

  void push_fastbin(Chunk *chunk, Chunk *&bin) {
    assert(chunk && !chunk->prev_);
    chunk->next_ = bin;
    bin = chunk;

    // we mark CHUNK_IN_USE for fastbin chunks
    // so that they won't be consolidated.
    set_flags(chunk, CHUNK_IN_USE);
  }

  Chunk *try_alloc_fastbin(size_t num_bytes) {
    size_t fast_bin_index = fast_index(num_bytes);
    assert(fast_bin_index < num_fast_bins_);
    auto chunk = pop_fastbin(fast_bins_[fast_bin_index]);
    assert(!chunk || get_flags(chunk) == CHUNK_IN_USE);
    return chunk;
  }

  Chunk *try_alloc_smallbin(size_t num_bytes) {
    assert(num_bytes >= min_small_size_);
    size_t small_bin_index = small_index(num_bytes);
    assert(small_bin_index < num_small_bins_);
    auto chunk = pop_front_from_bin(small_bins_[small_bin_index]);
    if (chunk) {
      set_flags(chunk, CHUNK_IN_USE);
    }
    return chunk;
  }

  void unlink_chunk(Chunk *chunk) {
    assert(chunk);
    assert(!(get_flags(chunk) & CHUNK_IN_USE));
    auto num_bytes = chunk->size_;
    if (get_flags(chunk) & CHUNK_UNSORTED) {
      assert(unsorted_bin_);
      set_flags(chunk, 0);
      unlink_from_bin(chunk, unsorted_bin_);
    } else if (num_bytes < min_small_size_) {
      assert(false);
    } else if (num_bytes < min_large_size_) {
      size_t small_bin_index = small_index(num_bytes);
      assert(small_bin_index < num_small_bins_ && small_bins_[small_bin_index]);
      unlink_from_bin(chunk, small_bins_[small_bin_index]);
    } else if (chunk != top_chunk_) {
      auto where = large_iter(num_bytes);
      assert(where != large_bin_.end() && (*where)->size_ == num_bytes);
      while (*where != chunk) {
        ++where;
      }
      assert(where != large_bin_.end() && *where == chunk);
      large_bin_.erase(where);
    } else {
      assert(chunk == top_chunk_);
    }
  }

  void consolidate_chunk(Chunk *chunk) {
    assert(chunk);
    auto size = chunk->size_;
    auto next = next_chunk(chunk);
    while (!is_begin(chunk) && !(chunk->prev_size_ & CHUNK_IN_USE)) {
      auto prev = prev_chunk(chunk);
      assert(prev->size_);
      unlink_chunk(prev);
      assert(get_flags(prev) == 0);
      size += prev->size_;
      chunk = prev;
    }
    bool next_was_top = false;
    while (next && !(next->size_ & CHUNK_IN_USE)) {
      assert(next->size_);
      unlink_chunk(next);
      assert(get_flags(next) == 0);
      size += next->size_;
      next_was_top = (next == top_chunk_);
      next = next_chunk(next);
    }
    if (!next_was_top) {
      set_size(chunk, size);
      push_front_to_bin(chunk, unsorted_bin_);
      set_flags(chunk, CHUNK_UNSORTED);
    } else {
      top_chunk_ = chunk;
      set_size(chunk, size);
    }
  }

  void consolidate_fastbin() {
    for (size_t i = 0; i < num_fast_bins_; ++i) {
      while (fast_bins_[i]) {
        auto chunk = pop_fastbin(fast_bins_[i]);
        set_flags(chunk, 0);
        consolidate_chunk(chunk);
      }
    }
  }

  void place_in_bins(Chunk *chunk) {
    assert(chunk && get_flags(chunk) == 0);
    auto num_bytes = chunk->size_;
    if (num_bytes < min_small_size_) {
      push_fastbin(chunk, fast_bins_[fast_index(num_bytes)]);
    } else if (num_bytes < min_large_size_) {
      push_back_to_bin(chunk, small_bins_[small_index(num_bytes)]);
    } else {
      large_bin_.insert(large_iter(num_bytes), chunk);
    }
  }

  Chunk *split_chunk_after(Chunk *chunk, size_t offset) {
    assert(chunk && (chunk != top_chunk_) && offset >= chunk_alignment_ &&
           (offset % chunk_alignment_) == 0 &&
           realsize(chunk) >= offset + sizeof(Chunk) &&
           (realsize(chunk) - offset) % chunk_alignment_ == 0);

    auto split_size = realsize(chunk) - offset;

    // set new size of the current (left) chunk and erase flags
    set_size(chunk, offset);

    Chunk *split_chunk = reinterpret_cast<Chunk *>(
        reinterpret_cast<unsigned char *>(chunk) + offset);
    assert(split_chunk->prev_size_ == offset);
    split_chunk->prev_ = nullptr;
    split_chunk->next_ = nullptr;
    set_size(split_chunk, split_size);
    assert(split_chunk == next_chunk(chunk));
    assert(verify_size(split_chunk) == split_size);
    return split_chunk;
  }

  Chunk *try_alloc_unsorted(size_t num_bytes) {
    while (unsorted_bin_) {
      auto chunk = pop_front_from_bin(unsorted_bin_);
      assert(chunk && get_flags(chunk) == CHUNK_UNSORTED);
      set_flags(chunk, 0);

      // for small requests, if this is the last unsorted,
      // then split and take if possible
      if (!unsorted_bin_ && num_bytes < min_large_size_ &&
          chunk->size_ >= num_bytes + chunk_alignment_) {
        auto remainder = split_chunk_after(chunk, num_bytes);
        set_flags(chunk, CHUNK_IN_USE);
        set_flags(remainder, CHUNK_UNSORTED);
        push_front_to_bin(remainder, unsorted_bin_);
        return chunk;
      }

      // use this if exact fit
      if (chunk->size_ == num_bytes) {
        set_flags(chunk, CHUNK_IN_USE);
        return chunk;
      }

      place_in_bins(chunk);
    }
    return nullptr;
  }

  Chunk *try_alloc_largebin(size_t num_bytes) {
    assert(num_bytes >= min_large_size_ && (num_bytes % chunk_alignment_) == 0);
    auto where = large_iter(num_bytes);

    if (where == large_bin_.end()) {
      // no large bin chunk is available
      return nullptr;
    } else {
      // best fit
      auto chunk = *where;
      assert(get_flags(chunk) == 0);
      assert(chunk->size_ >= num_bytes);
      large_bin_.erase(where);
      if (chunk->size_ == num_bytes) {
        // use if exact fit
        set_flags(chunk, CHUNK_IN_USE);
        return chunk;
      } else if (chunk->size_ >= num_bytes + chunk_alignment_) {
        assert((chunk->size_ - num_bytes) % chunk_alignment_ == 0);
        // split to take remainder
        auto remainder = split_chunk_after(chunk, num_bytes);
        set_flags(chunk, CHUNK_IN_USE);
        place_in_bins(remainder);
        return chunk;
      }
    }
  }

  Chunk *alloc_top(size_t num_bytes) {
    assert(num_bytes >= chunk_alignment_ &&
           (num_bytes % chunk_alignment_) == 0);
    assert(get_flags(top_chunk_) == 0);
    if (top_chunk_->size_ < num_bytes + chunk_alignment_) {
      // TODO: how to deal with this?
      cout << "Oh no\n";
      throw runtime_error("memory pool is not enough");
    }

    assert((top_chunk_->size_ % chunk_alignment_ == 0) &&
           top_chunk_->size_ >= num_bytes + chunk_alignment_ &&
           (top_chunk_->size_ - num_bytes) % chunk_alignment_ == 0);

    auto chunk = top_chunk_;

    auto split_size = top_chunk_->size_ - num_bytes;
    // set new size of the current (left) chunk and erase flags
    top_chunk_->size_ = num_bytes;
    Chunk *split_chunk = reinterpret_cast<Chunk *>(
        reinterpret_cast<unsigned char *>(top_chunk_) + num_bytes);
    assert(split_chunk);
    size_t *foot = reinterpret_cast<size_t *>(split_chunk);
    *foot = num_bytes;
    assert(split_chunk->prev_size_ == num_bytes);
    split_chunk->prev_ = nullptr;
    split_chunk->next_ = nullptr;
    split_chunk->size_ = split_size;
    top_chunk_ = split_chunk;
    assert(split_chunk == next_chunk(chunk));
    assert(split_chunk == top_chunk_);
    set_flags(chunk, CHUNK_IN_USE);
    return chunk;
  }

  size_t request_to_sz(size_t num_bytes) {
    return ((num_bytes / chunk_alignment_) * chunk_alignment_) +
           ((num_bytes % chunk_alignment_) ? chunk_alignment_ : 0);
  }

public:
  void *alloc_chunk(size_t num_bytes) {
    num_bytes = request_to_sz(num_bytes);
    assert(num_bytes >= min_fast_size_ && num_bytes % chunk_alignment_ == 0);
    if (num_bytes < min_small_size_) {
      auto res = try_alloc_fastbin(num_bytes);
      if (res) {
        assert(res->size_ == (num_bytes | CHUNK_IN_USE));
        assert(verify());
        return reinterpret_cast<void *>(res);
      }
    } else if (num_bytes < min_large_size_) {
      auto res = try_alloc_smallbin(num_bytes);
      if (res) {
        assert(res->size_ == (num_bytes | CHUNK_IN_USE));
        assert(verify());
        return reinterpret_cast<void *>(res);
      }
    } else {
      consolidate_fastbin();
      assert(verify());
    }
    {
      auto res = try_alloc_unsorted(num_bytes);
      if (res) {
        assert(res->size_ == (num_bytes | CHUNK_IN_USE));
        assert(verify());
        return reinterpret_cast<void *>(res);
      }
    }
    auto res = alloc_top(num_bytes);
    assert(res && res->size_ == (num_bytes | CHUNK_IN_USE));
    assert(verify());
    return reinterpret_cast<void *>(res);
  }

  void dealloc_chunk(void *ptr) {
    assert(ptr);
    Chunk *chunk = reinterpret_cast<Chunk *>(ptr);
    assert(get_flags(chunk) == CHUNK_IN_USE);
    set_flags(chunk, 0);
    assert(chunk != top_chunk_);
    if (next_chunk(chunk) != top_chunk_ && chunk->size_ < min_small_size_) {
      push_fastbin(chunk, fast_bins_[fast_index(chunk->size_)]);
    } else {
      consolidate_chunk(chunk);
    }
    assert(verify());
  }
};

} // namespace frozenca

#endif //__CLRS4_ALLOC_MANAGER_DEFAULT__
