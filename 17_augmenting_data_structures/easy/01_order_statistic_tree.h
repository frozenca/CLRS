#ifndef __CLRS4_ORDER_STATISTIC_TREE_H__
#define __CLRS4_ORDER_STATISTIC_TREE_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <ranges>
#include <rbtree.h>
#include <tuple>
#include <vector>

namespace frozenca {

using namespace std;

template <Containable T>
struct RBOrderNode : public hard::detail::RBTreeNodeBase<T, RBOrderNode<T>> {
  using Base = hard::detail::RBTreeNodeBase<T, RBOrderNode<T>>;
  ptrdiff_t size_ = 1;

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }

  RBOrderNode *left() { return base().left_.get(); }

  const RBOrderNode *left() const { return base().left_.get(); }

  RBOrderNode *right() { return base().right_.get(); }

  const RBOrderNode *right() const { return base().right_.get(); }

  [[nodiscard]] ptrdiff_t correct_size() const noexcept {
    return (left() ? left()->size_ : 0) + (right() ? right()->size_ : 0) + 1;
  }
};

template <Containable K>
class OrderStatisticTree
    : public hard::detail::RedBlackTreeBase<
          K, K, less<K>, false, RBOrderNode<K>, OrderStatisticTree<K>> {
public:
  using Base =
      hard::detail::RedBlackTreeBase<K, K, less<K>, false, RBOrderNode<K>,
                                     OrderStatisticTree<K>>;
  friend class Base;
  using Comp = less<K>;
  using Node = RBOrderNode<K>;
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

    assert(node->size_ == node->correct_size());

    // to mute unused variable right_black_height compiler warning for release
    // builds..
    return (left_black_height + right_black_height) / 2;
  }

public:
  void verify() const {
    // the root is black.
    assert(!this->get_root() || this->get_root()->black_);
    assert(base().bh_ == verify(this->get_root()));
  }

protected:
  const Node *os_select(const Node *node, ptrdiff_t i) const {
    assert(node);
    auto r = ((node->left_) ? node->left_->size_ : 0);
    if (i == r) {
      return node;
    } else if (i < r) {
      return os_select(node->left_.get(), i);
    } else {
      return os_select(node->right_.get(), i - r - 1);
    }
  }

  const Node *os_select_iterative(ptrdiff_t i) const {
    auto node = this->get_root();
    while (true) {
      auto r = ((node->left_) ? node->left_->size_ : 0);
      if (i == r) {
        return node;
      } else if (i < r) {
        node = node->left();
      } else {
        node = node->right();
        i -= (r + 1);
      }
    }
  }

  ptrdiff_t os_rank(const Node *node) const {
    assert(node);
    auto r = ((node->left_) ? node->left_->size_ : 0);
    auto y = node;
    while (y != this->get_root()) {
      if (y == y->parent_->right()) {
        r += ((y->parent_->left_) ? (y->parent_->left_->size_) : 0) + 1;
      }
      y = y->parent_;
    }
    return r;
  }

private:
  void left_rotate_post(Node *y, Node *x) {
    if (x && y) {
      y->size_ = x->size_;
      x->size_ = x->correct_size();
    }
  }

  void right_rotate_post(Node *y, Node *x) {
    if (x && y) {
      y->size_ = x->size_;
      x->size_ = x->correct_size();
    }
  }

  void insert_fixup_pre(Node *, Node *zp) {
    auto curr = zp;
    while (curr) {
      curr->size_++;
      curr = curr->parent_;
    }
  }

  void erase_fixup_x(Node *, Node *zp, const hard::detail::BSTChild &) {
    auto curr = zp;
    while (curr) {
      curr->size_--;
      curr = curr->parent_;
    }
  }

  void erase_fixup_zy(Node *z, Node *y) { y->size_ = z->size_; }

public:
  reference_type operator[](ptrdiff_t i) {
    return const_cast<Node *>(os_select(this->get_root(), i))->key_;
  }

  const_reference_type operator[](ptrdiff_t i) const {
    return os_select(this->get_root(), i)->key_;
  }

  reference_type at(ptrdiff_t i) {
    return const_cast<Node *>(os_select_iterative(i))->key_;
  }

  const_reference_type at(ptrdiff_t i) const {
    return os_select_iterative(i)->key_;
  }

  [[nodiscard]] ptrdiff_t index(const K &key) const {
    auto node = this->find_node(key);
    if (!node) {
      return -1;
    } else {
      return os_rank(node);
    }
  }

  const K &succ(const K &key, ptrdiff_t i) const {
    auto idx = index(key);
    if (idx == -1) {
      return key;
    } else {
      return at(idx + i);
    }
  }

  friend index_t inversions_tree(const vector<K> &nums);

  friend index_t num_pairs_intersecting(vector<pair<float, float>> &chords);
};

template <Containable K> index_t inversions_tree(const vector<K> &nums) {
  OrderStatisticTree<K> tree;
  index_t i = 0;
  index_t res = 0;
  for (auto num : nums) {
    tree.insert(num);
    auto idx = tree.index(num);
    res += i - idx;
    i++;
  }
  return res;
}

index_t num_pairs_intersecting(vector<pair<float, float>> &chords) {
  vector<tuple<float, float, bool>> endpoints;
  for (auto &[a, b] : chords) {
    if (a > b) {
      swap(a, b);
    }
    endpoints.emplace_back(a, a, true);
    endpoints.emplace_back(b, a, false);
  }
  ranges::sort(endpoints);

  OrderStatisticTree<float> tree;
  index_t count = 0;
  for (const auto &[p, q, left] : endpoints) {
    if (left) {
      tree.insert(p);
    } else {
      tree.erase(q);
      index_t sz = tree.get_root() ? tree.get_root()->size_ : 0;
      count += sz;
    }
  }

  return count;
}

} // namespace frozenca

#endif //__CLRS4_ORDER_STATISTIC_TREE_H__
