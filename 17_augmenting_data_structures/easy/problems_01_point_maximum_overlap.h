#ifndef __CLRS4_POINT_MAXIMUM_OVERLAP_H__
#define __CLRS4_POINT_MAXIMUM_OVERLAP_H__

#include <17_augmenting_data_structures/easy/02_associative_attr_tree.h>
#include <interval.h>

namespace frozenca {

using namespace std;

struct EndpointAttr {
  index_t p_ = 0;
  index_t sum_ = 0;
  index_t max_ = 0;
  Interval where_ = {};
};

template <Containable T> struct Endpoint {
  T point_ = {};
  bool is_right_ = false;
  Interval origin_ = {};
};

struct EndpointComp {
  template <Containable T>
  partial_ordering operator()(const Endpoint<T> &pt1, const Endpoint<T> &pt2) {
    if (auto cmp = pt1.point_ <=> pt2.point_;
        cmp != partial_ordering::equivalent) {
      return cmp;
    }
    return pt1.is_right_ <=> pt2.is_right_;
  }
};

struct SetEndpointAttr {
  template <Containable T>
  constexpr EndpointAttr operator()(const Endpoint<T> &key) const noexcept {
    EndpointAttr attr;
    attr.p_ = key.is_right_ ? -1 : 1;
    attr.where_ = key.origin_;
    return attr;
  }
};

struct ValidateEndpointAttr {
  constexpr EndpointAttr operator()(const EndpointAttr &l,
                                    const EndpointAttr &m,
                                    const EndpointAttr &r) const noexcept {
    EndpointAttr new_attr;
    new_attr.p_ = m.p_;
    new_attr.sum_ = l.sum_ + m.p_ + r.sum_;
    new_attr.max_ = max({l.max_, l.sum_ + m.p_, l.sum_ + m.p_ + r.max_});
    if (new_attr.max_ == l.max_) {
      new_attr.where_ = l.where_;
    } else if (new_attr.max_ == l.sum_ + m.p_) {
      new_attr.where_ = m.where_;
    } else {
      new_attr.where_ = r.where_;
    }
    return new_attr;
  }
};

template <Containable T>
struct EndpointAttrNode
    : public hard::detail::RBTreeNodeBase<Endpoint<T>, EndpointAttrNode<T>> {
  using Base = hard::detail::RBTreeNodeBase<Endpoint<T>, EndpointAttrNode<T>>;
  using AttrType = EndpointAttr;
  using attr_type = AttrType;
  AttrType attr_ = {};

  EndpointAttrNode() : Base() { attr_ = get_default(); }

  EndpointAttrNode(const Endpoint<T> &key) : Base(key) {
    attr_ = get_default();
  }

  AttrType get_default() const noexcept {
    return SetEndpointAttr{}(this->key_);
  }

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }

  EndpointAttrNode *left() { return base().left_.get(); }

  const EndpointAttrNode *left() const { return base().left_.get(); }

  EndpointAttrNode *right() { return base().right_.get(); }

  const EndpointAttrNode *right() const { return base().right_.get(); }

  [[nodiscard]] AttrType correct_attr() const noexcept {
    return ValidateEndpointAttr{}(left() ? left()->attr_ : AttrType{},
                                  get_default(),
                                  right() ? right()->attr_ : AttrType{});
  }
};

class EndpointTree
    : public AttrTreeBase<Endpoint<float>, EndpointComp,
                          EndpointAttrNode<float>, EndpointTree> {
public:
  void insert_interval(const Interval &itv) {
    this->insert({itv.first_, false, itv});
    this->insert({itv.second_, true, itv});
  }
  
  void erase_interval(const Interval &itv) {
    this->erase({itv.first_, false, itv});
    this->erase({itv.second_, true, itv});
  }
};

} // namespace frozenca

#endif //__CLRS4_POINT_MAXIMUM_OVERLAP_H__
