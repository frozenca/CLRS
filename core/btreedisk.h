#ifndef __CLRS4_BTREE_DISK_H__
#define __CLRS4_BTREE_DISK_H__

#include <algorithm>
#include <allocatorfixed.h>
#include <btree.h>
#include <cassert>
#include <common.h>
#include <ranges>
#include <vector>


namespace frozenca {

using namespace std;

template <Allocable K, index_t t = 128, typename Comp = ranges::less>
using DiskBTree =
    detail::BTreeBase<K, K, t, t, Comp, false, AllocatorFixed<K, 2 * t - 1>>;

} // namespace frozenca

#endif //__CLRS4_BTREE_DISK_H__
