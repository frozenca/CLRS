#ifndef __CLRS4_ATTR_TREE_H__
#define __CLRS4_ATTR_TREE_H__

#include <algorithm>
#include <cassert>
#include <ranges>
#include <rbtree.h>
#include <string>
#include <vector>

namespace frozenca {

using namespace std;

template <typename F, typename T>
concept BinaryOp = requires(F &&f, T &&t1, T &&t2) {
  {
    invoke(forward<F>(f), forward<T>(t1), forward<T>(t2))
    } -> convertible_to<T>;
};

template <Containable T> struct SetKey {
  constexpr T operator()(const T &key) const noexcept { return key; }
};

template <Containable T> struct SetOne {
  constexpr index_t operator()(const T &) const noexcept { return 1; }
};

template <Containable T, typename AttrType, typename F, typename SetDefault>
requires(BinaryOp<F, AttrType> &&is_convertible_v<
         invoke_result_t<SetDefault, T>, AttrType>) struct AttrNode
    : public detail::RBTreeNodeBase<
          T, AttrNode<T, AttrType, F, SetDefault>> {
  using Base =
      detail::RBTreeNodeBase<T, AttrNode<T, AttrType, F, SetDefault>>;
  using attr_type = AttrType;
  AttrType attr_ = {};

  AttrNode() : Base() { attr_ = get_default(); }

  AttrNode(const T &key) : Base(key) { attr_ = get_default(); }

  AttrType get_default() const noexcept { return SetDefault{}(this->key_); }

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }

  AttrNode *left() { return base().left_.get(); }

  const AttrNode *left() const { return base().left_.get(); }

  AttrNode *right() { return base().right_.get(); }

  const AttrNode *right() const { return base().right_.get(); }

  [[nodiscard]] AttrType correct_attr() const noexcept {
    return F{}(F{}((left() ? left()->attr_ : AttrType{}), get_default()),
               (right() ? right()->attr_ : AttrType{}));
  }
};

template <Containable K, typename Comp, typename AttrNodeType, typename Derived>
class AttrTreeBase : public detail::RedBlackTreeBase<
                         K, K, Comp, false, AttrNodeType,
                         AttrTreeBase<K, Comp, AttrNodeType, Derived>> {
public:
  using Base = detail::RedBlackTreeBase<
      K, K, Comp, false, AttrNodeType,
      AttrTreeBase<K, Comp, AttrNodeType, Derived>>;
  friend class Base;
  using Node = AttrNodeType;
  using key_type = Base::key_type;
  using reference_type = Base::reference_type;
  using const_reference_type = Base::const_reference_type;
  using iterator_type = Base::iterator_type;
  using const_iterator_type = Base::const_iterator_type;
  using reverse_iterator_type = Base::reverse_iterator_type;
  using const_reverse_iterator_type = Base::const_reverse_iterator_type;
  using attr_type = Node::attr_type;

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

    assert(node->attr_ == node->correct_attr());

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

  Derived &derived() { return static_cast<Derived &>(*this); }

  const Derived &derived() const { return static_cast<const Derived &>(*this); }

protected:
  void left_rotate_post(Node *y, Node *x) {
    if (x) {
      x->attr_ = x->correct_attr();
    }
    if (y) {
      y->attr_ = y->correct_attr();
    }
  }

  void right_rotate_post(Node *y, Node *x) {
    if (x) {
      x->attr_ = x->correct_attr();
    }
    if (y) {
      y->attr_ = y->correct_attr();
    }
  }

  void insert_fixup_pre(Node *, Node *zp) {
    auto curr = zp;
    while (curr) {
      curr->attr_ = curr->correct_attr();
      curr = curr->parent_;
    }
  }

  void erase_fixup_x(Node *, Node *zp, const detail::BSTChild &) {
    auto curr = zp;
    while (curr) {
      curr->attr_ = curr->correct_attr();
      curr = curr->parent_;
    }
  }

public:
  [[nodiscard]] attr_type get_attr() const noexcept {
    return this->get_root() ? this->get_root()->attr_ : attr_type{};
  }
};

template <Containable K, typename Comp, typename T, typename F,
          typename SetDefault>
class AttrTree : public AttrTreeBase<K, Comp, AttrNode<K, T, F, SetDefault>,
                                     AttrTree<K, Comp, T, F, SetDefault>> {};

template <Containable K>
using AttrStatisticTree =
    AttrTree<K, compare_three_way, index_t, plus<index_t>, SetOne<K>>;

using StringConcatTree =
    AttrTree<string, compare_three_way, string, plus<string>, SetKey<string>>;

} // namespace frozenca

#endif //__CLRS4_ATTR_TREE_H__
