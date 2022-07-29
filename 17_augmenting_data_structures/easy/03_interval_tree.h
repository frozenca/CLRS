#ifndef __CLRS4_INTERVAL_TREE_H__
#define __CLRS4_INTERVAL_TREE_H__

#include <17_augmenting_data_structures/easy/02_associative_attr_tree.h>
#include <interval.h>

namespace frozenca {

using namespace std;

struct IntervalSetDefaultMax {
  constexpr float operator()(const Interval &itv) const noexcept {
    return itv.second_;
  }
};

class IntervalTree
    : public AttrTreeBase<
          Interval, IntervalStartComp,
          AttrNode<Interval, float, Max<float>, IntervalSetDefaultMax>,
          IntervalTree> {
public:
  using Base =
      AttrTreeBase<Interval, IntervalStartComp,
                   AttrNode<Interval, float, Max<float>, IntervalSetDefaultMax>,
                   IntervalTree>;
  friend class Base;

  using Node = Base::Node;
  using Comp = IntervalStartComp;
  using key_type = Base::key_type;
  using reference_type = Base::reference_type;
  using const_reference_type = Base::const_reference_type;
  using iterator_type = Base::iterator_type;
  using const_iterator_type = Base::const_iterator_type;
  using reverse_iterator_type = Base::reverse_iterator_type;
  using const_reverse_iterator_type = Base::const_reverse_iterator_type;
  using SearchResult = detail::BSTSearchResult<Node>;

protected:
  Node *find_node_exact(const Interval &key) const {
    auto x = this->get_root();
    while (x) {
      if (x->key_ == key) {
        return const_cast<Node *>(x);
      } else if (x->key_.first_ <= key.first_) {
        x = x->right_.get();
      } else {
        x = x->left_.get();
      }
    }
    return const_cast<Node *>(x);
  }

  Node *find_overlapping_lower_bound(const Interval &key) const {
    auto x = this->get_root();
    Node *bound = nullptr;
    while (x) {
      if (x->left_ && x->left_->attr_ < key.first_) {
        x = x->right_.get();
      } else {
        bound = const_cast<Node *>(x);
        x = x->left_.get();
      }
    }
    return bound;
  }

  Node *find_overlapping_upper_bound(const Interval &key) const {
    auto x = this->get_root();
    Node *bound = nullptr;
    while (x) {
      if (x->key_.first_ > key.second_) {
        bound = const_cast<Node *>(x);
        x = x->left_.get();
      } else {
        x = x->right_.get();
      }
    }
    return bound;
  }

public:
  bool find_exact(const Interval &key) const {
    return find_node_exact(key) != nullptr;
  }

  void erase_exact(const Interval &key) { this->erase(find_node_exact(key)); }

  iterator_type overlapping_lower_bound(const Interval &key) {
    return iterator_type(find_overlapping_lower_bound(key));
  }

  const_iterator_type overlapping_lower_bound(const Interval &key) const {
    return const_iterator_type(find_overlapping_lower_bound(key));
  }

  iterator_type overlapping_upper_bound(const Interval &key) {
    return iterator_type(find_overlapping_upper_bound(key));
  }

  const_iterator_type overlapping_upper_bound(const Interval &key) const {
    return const_iterator_type(find_overlapping_upper_bound(key));
  }

  bool overlaps(const Interval &key) const {
    return overlapping_lower_bound(key) != overlapping_upper_bound(key);
  }

  pair<iterator_type, iterator_type>
  overlapping_equal_range(const Interval &key) {
    return {overlapping_lower_bound(key), overlapping_upper_bound(key)};
  }

  pair<const_iterator_type, const_iterator_type>
  overlapping_equal_range(const Interval &key) const {
    return {overlapping_lower_bound(key), overlapping_upper_bound(key)};
  }
};

} // namespace frozenca

#endif //__CLRS4_INTERVAL_TREE_H__
