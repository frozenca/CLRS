#ifndef __CLRS4_COMMON_H__
#define __CLRS4_COMMON_H__

#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

namespace frozenca {

using namespace std;

using index_t = ptrdiff_t;
using uindex_t = make_unsigned_t<index_t>;

namespace {

constexpr size_t arr_default_length = 128;

} // anonymous namespace
} // namespace frozenca

#endif //__CLRS4_COMMON_H__
