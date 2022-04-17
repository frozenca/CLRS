#ifndef __CLRS4_HEAPS_H__
#define __CLRS4_HEAPS_H__

#include <cassert>
#include <common.h>

namespace frozenca {

using namespace std;

inline constexpr index_t heap_parent(index_t i) {
    assert(i >= 0);
    return i / 2;
}

inline constexpr index_t heap_left(index_t i) {
    assert(i >= 0);
    return 2 * i + 1;
}

inline constexpr index_t heap_right(index_t i) {
    assert(i >= 0);
    return 2 * i + 2;
}

} // namespace frozenca

#endif //__CLRS4_HEAPS_H__
