#ifndef __CLRS4_BH_MAINTAIN_TREE_H__
#define __CLRS4_BH_MAINTAIN_TREE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <rbtree.h>
#include <vector>

namespace frozenca {

using namespace std;

template <Containable T>
struct RBBHNode : public hard::detail::RBTreeNodeBase<T, RBBHNode<T>> {
  using Base = hard::detail::RBTreeNodeBase<T, RBBHNode<T>>;

  ptrdiff_t bh_ = 0;

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }
};

template <Containable K>
class BHTree : public hard::detail::RedBlackTreeBase<K, K, less<K>, false,
                                                     RBBHNode<K>, BHTree<K>> {
public:
  using Base = hard::detail::RedBlackTreeBase<K, K, less<K>, false, RBBHNode<K>,
                                              BHTree<K>>;
  friend class Base;
  using Comp = less<K>;
  using Node = RBBHNode<K>;
  using key_type = Base::key_type;
  using reference_type = Base::reference_type;
  using const_reference_type = Base::const_reference_type;

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }

private:
  static int verify(const Node *node) {
    if (!node) {
      return 0;
    }

    auto left_black_height = verify(node->left_.get());
    auto right_black_height = verify(node->right_.get());

    if (node->left_ && node->left_->black_) {
      left_black_height++;
    }
    if (node->right_ && node->right_->black_) {
      right_black_height++;
    }

    // binary search tree properties
    assert(!node->left_ ||
           (node->left_->parent_ == node && node->key_ >= node->left_->key_));
    assert(!node->right_ ||
           (node->right_->parent_ == node && node->key_ <= node->right_->key_));

    // if a node is red, then both its children are black.
    assert(node->black_ || ((!node->left_ || node->left_->black_) &&
                            (!node->right_ || node->right_->black_)));

    // for each node, all simple paths from the node to
    // descendant leaves contain the same number of black nodes.
    assert(left_black_height == right_black_height);

    // to mute unused variable right_black_height compiler warning for release
    // builds..
    auto bh = (left_black_height + right_black_height) / 2;
    assert(node->bh_ == bh);
    return bh;
  }

public:
  void verify() const {
    // the root is black.
    assert(!this->get_root() || this->get_root()->black_);
  }

protected:
  void erase_fixup_zy(Node *z, Node *y) { y->bh_ = z->bh_; }

  void adjust_bh(Node *x, ptrdiff_t i) {
    if (x) {
      x->bh_ += i;
    }
  }

  void adjust_bh(Node *x, const Node *ref, ptrdiff_t i) {
    x->bh_ = (ref ? ref->bh_ : 0) + i;
  }
};

} // namespace frozenca

#endif //__CLRS4_BH_MAINTAIN_TREE_H__
