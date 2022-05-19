#ifndef __CLRS4_DEQUEUE_H__
#define __CLRS4_DEQUEUE_H__

#include <array>
#include <common.h>
#include <stdexcept>

namespace frozenca {

using namespace std;

template <typename T, size_t N = arr_default_length> class Dequeue {
  array<T, N> data;
  index_t head = 0;
  index_t tail = 0;
  index_t size = 0;

public:
  void enqueue_front(const T &x) {
    if (size == N) {
      throw overflow_error("overflow");
    }
    if (head == 0) {
      head = N - 1;
    } else {
      head--;
    }
    data[head] = x;
    size++;
  }

  void enqueue_back(const T &x) {
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

  T dequeue_front() {
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

  T dequeue_back() {
    if (size == 0) {
      throw underflow_error("underflow");
    }
    if (tail == 0) {
      tail = N - 1;
    } else {
      tail--;
    }
    auto x = data[tail];
    size--;
    return x;
  }
};

} // namespace frozenca

#endif //__CLRS4_DEQUEUE_H__
