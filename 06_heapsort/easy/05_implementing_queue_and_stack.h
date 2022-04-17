#ifndef __CLRS4_IMPLEMENTING_QUEUE_AND_STACK_H__
#define __CLRS4_IMPLEMENTING_QUEUE_AND_STACK_H__

#include "05_priority_queues.h"
#include <algorithm>
#include <cassert>
#include <common.h>
#include <utility>

namespace frozenca {

using namespace std;

class PQQueue {
public:
  struct Comp {
    bool operator()(const pair<int, index_t> &p1,
                    const pair<int, index_t> &p2) const {
      return p1.second > p2.second;
    }
  };

  void enqueue(int x) {
    pq.emplace_back(x, index);
    index++;
    ranges::push_heap(pq, comp);
  }

  int dequeue() {
    ranges::pop_heap(pq, comp);
    auto [x, ind] = pq.back();
    pq.pop_back();
    return x;
  }

private:
  vector<pair<int, index_t>> pq;
  index_t index = 0;
  Comp comp;
};

class PQStack {
public:
  struct Comp {
    bool operator()(const pair<int, index_t> &p1,
                    const pair<int, index_t> &p2) const {
      return p1.second < p2.second;
    }
  };

  void push(int x) {
    pq.emplace_back(x, index);
    index++;
    ranges::push_heap(pq, comp);
  }

  int pop() {
    ranges::pop_heap(pq, comp);
    auto [x, ind] = pq.back();
    pq.pop_back();
    return x;
  }

private:
  vector<pair<int, index_t>> pq;
  index_t index = 0;
  Comp comp;
};

} // namespace frozenca

#endif //__CLRS4_IMPLEMENTING_QUEUE_AND_STACK_H__
