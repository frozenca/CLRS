#ifndef __CLRS4_MIN_GAP_TREE_H__
#define __CLRS4_MIN_GAP_TREE_H__

#include <17_augmenting_data_structures/easy/02_associative_attr_tree.h>
#include <limits>


namespace frozenca {

using namespace std;

template <Containable T> struct GapAttr {
  T min_gap_ = numeric_limits<T>::max();
  T min_val_ = T{};
  T val_ = T{};
  T max_val_ = T{};
  bool is_null_ = true;
  GapAttr() = default;
  GapAttr(const T &val)
      : min_val_{val}, val_{val}, max_val_{val}, is_null_{false} {}
};

struct SetGapAttr {
  template <Containable T>
  constexpr GapAttr<T> operator()(const T &key) const noexcept {
    return GapAttr<T>(key);
  }
};

struct ValidateGapAttr {
  template <Containable T>
  constexpr GapAttr<T> operator()(const GapAttr<T> &l, const GapAttr<T> &m,
                                  const GapAttr<T> &r) const noexcept {
    GapAttr<T> new_attr(m.val_);
    new_attr.min_val_ = l.is_null_ ? m.val_ : l.min_val_;
    new_attr.max_val_ = r.is_null_ ? m.val_ : r.max_val_;
    new_attr.min_gap_ =
        min({l.min_gap_, r.min_gap_,
             l.is_null_ ? numeric_limits<T>::max() : m.val_ - l.max_val_,
             r.is_null_ ? numeric_limits<T>::max() : r.min_val_ - m.val_});
    return new_attr;
  }
};

template <Containable T>
struct GapNode : public hard::detail::RBTreeNodeBase<T, GapNode<T>> {
  using Base = hard::detail::RBTreeNodeBase<T, GapNode<T>>;
  using AttrType = GapAttr<T>;
  using attr_type = AttrType;
  AttrType attr_ = {};

  GapNode() : Base() { attr_ = get_default(); }

  GapNode(const T &key) : Base(key) { attr_ = get_default(); }

  AttrType get_default() const noexcept { return SetGapAttr{}(this->key_); }

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }

  GapNode *left() { return base().left_.get(); }

  const GapNode *left() const { return base().left_.get(); }

  GapNode *right() { return base().right_.get(); }

  const GapNode *right() const { return base().right_.get(); }

  [[nodiscard]] AttrType correct_attr() const noexcept {
    return ValidateGapAttr{}(left() ? left()->attr_ : AttrType{}, get_default(),
                             right() ? right()->attr_ : AttrType{});
  }
};

template <Containable K>
class GapTree
    : public AttrTreeBase<K, compare_three_way, GapNode<K>, GapTree<K>> {};

} // namespace frozenca

#endif //__CLRS4_MIN_GAP_TREE_H__
