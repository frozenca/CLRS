#ifndef __CLRS4_TWO_STACKS_H__
#define __CLRS4_TWO_STACKS_H__

#include <array>
#include <common.h>
#include <stdexcept>

namespace frozenca {

using namespace std;

template <typename T, size_t N = arr_default_length> class TwoStacks {
  array<T, N> data;
  index_t top1 = 0;
  index_t top2 = N;

public:
  [[nodiscard]] bool stack1_empty() const { return top1 == 0; }
  [[nodiscard]] bool stack2_empty() const { return top2 == N; }

  void push1(const T &x) {
    if (top1 == top2) {
      throw overflow_error("overflow\n");
    }
    data[top1] = x;
    top1++;
  }

  void push2(const T &x) {
    if (top1 == top2) {
      throw overflow_error("overflow\n");
    }
    top2--;
    data[top2] = x;
  }

  T pop1() {
    if (stack1_empty()) {
      throw underflow_error("underflow\n");
    }
    --top1;
    return data[top1];
  }

  T pop2() {
    if (stack2_empty()) {
      throw underflow_error("underflow\n");
    }
    auto elem = data[top2];
    top2++;
    return elem;
  }
};

} // namespace frozenca

#endif //__CLRS4_TWO_STACKS_H__
