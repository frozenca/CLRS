#ifndef __CLRS4_POINTER_MAINTAIN_TREE_H__
#define __CLRS4_POINTER_MAINTAIN_TREE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <rbtree.h>
#include <vector>

namespace frozenca {

using namespace std;

template <Containable T>
struct RBTrackNode : public detail::RBTreeNodeBase<T, RBTrackNode<T>> {
  using Base = detail::RBTreeNodeBase<T, RBTrackNode<T>>;

  RBTrackNode *succ_ = nullptr;
  RBTrackNode *pred_ = nullptr;

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }
};

template <Containable K>
class TrackTree
    : public detail::RedBlackTreeBase<K, K, compare_three_way, false,
                                            RBTrackNode<K>, TrackTree<K>> {
public:
  using Base = detail::RedBlackTreeBase<K, K, compare_three_way, false,
                                              RBTrackNode<K>, TrackTree<K>>;
  friend class Base;
  using Comp = compare_three_way;
  using Node = RBTrackNode<K>;
  using key_type = Base::key_type;
  using reference_type = Base::reference_type;
  using const_reference_type = Base::const_reference_type;

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }

private:
  Node *max_ = nullptr;
  Node *min_ = nullptr;

protected:
  void insert_fixup_pre(Node *z, Node *zp) {
    assert(z);
    if (zp && z == zp->left_.get()) {
      z->pred_ = zp->pred_;
      zp->pred_ = z;
      if (z->pred_) {
        z->pred_->succ_ = z;
      }
      z->succ_ = zp;
    } else if (zp && z == zp->right_.get()) {
      z->succ_ = zp->succ_;
      zp->succ_ = z;
      if (z->succ_) {
        z->succ_->pred_ = z;
      }
      z->pred_ = zp;
    }
    if (!min_ || z->key_ < min_->key_) {
      min_ = z;
    }
    if (!max_ || z->key_ > max_->key_) {
      max_ = z;
    }
  }

  void erase_fixup_z(Node *z, const detail::BSTChild&) {
    assert(z);
    if (z->pred_) {
      z->pred_->succ_ = z->succ_;
    }
    if (z->succ_) {
      z->succ_->pred_ = z->pred_;
    }
    if (max_ == z) {
      max_ = z->pred_;
    }
    if (min_ == z) {
      min_ = z->succ_;
    }
  }

public:
  [[nodiscard]] vector<K> track_sequence_fwd() const {
    vector<K> seq;
    auto curr = min_;
    for (; curr != max_; curr = curr->succ_) {
      if (curr) {
        seq.push_back(curr->key_);
      }
    }
    if (curr) {
      seq.push_back(curr->key_);
    }
    return seq;
  }

  [[nodiscard]] vector<K> track_sequence_bwd() const {
    vector<K> seq;
    auto curr = max_;
    for (; curr != min_; curr = curr->pred_) {
      if (curr) {
        seq.push_back(curr->key_);
      }
    }
    if (curr) {
      seq.push_back(curr->key_);
    }
    ranges::reverse(seq);
    return seq;
  }
};

} // namespace frozenca

#endif //__CLRS4_POINTER_MAINTAIN_TREE_H__
