#ifndef __CLRS4_UTIL_H__
#define __CLRS4_UTIL_H__

#include <ranges>
#include <iostream>

namespace frozenca {

using namespace std;

template <ranges::input_range R>
void print(R&& r, ostream& os = cout) {
    for (auto elem : r) {
        os << elem << ' ';
    }
    os << '\n';
}

} // namespace frozenca

#endif //__CLRS4_UTIL_H__
