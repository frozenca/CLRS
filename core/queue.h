#ifndef __CLRS4_QUEUE_H__
#define __CLRS4_QUEUE_H__

#include <array>
#include <common.h>
#include <stdexcept>

namespace frozenca {

using namespace std;

template <typename T, size_t N = arr_default_length> class Queue {
  array<T, N> data;
  index_t head = 0;
  index_t tail = 0;
  index_t size = 0;

public:
  void enqueue(const T &x) {
    if (size == N) {
      throw overflow_error("overflow");
    }
    data[tail] = x;
    if (tail == N - 1) {
      tail = 0;
    } else {
      tail++;
    }
    size++;
  }

  T dequeue() {
    if (size == 0) {
      throw underflow_error("underflow");
    }
    auto x = data[head];
    if (head == N - 1) {
      head = 0;
    } else {
      head++;
    }
    size--;
    return x;
  }
};

} // namespace frozenca

#endif //__CLRS4_QUEUE_H__
