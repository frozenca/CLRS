#ifndef __CLRS4_INTERVAL_TREE_H__
#define __CLRS4_INTERVAL_TREE_H__

#include <cassert>
#include <common.h>
#include <interval.h>
#include <rbtree.h>

namespace frozenca {

using namespace std;

struct IntervalNode : public hard::detail::RBTreeNodeBase<Interval, IntervalNode> {
  using Base = hard::detail::RBTreeNodeBase<Interval, IntervalNode>;
  ptrdiff_t rank_ = 0;

  Base &base() { return static_cast<Base &>(*this); }

  const Base &base() const { return static_cast<const Base &>(*this); }

  RBRankNode *left() { return base().left_.get(); }

  const RBRankNode *left() const { return base().left_.get(); }

  RBRankNode *right() { return base().right_.get(); }

  const RBRankNode *right() const { return base().right_.get(); }
};



} // namespace frozenca

#endif //__CLRS4_INTERVAL_TREE_H__
