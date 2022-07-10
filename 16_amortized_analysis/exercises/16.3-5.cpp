#include <10_elementary_data_structures/inc/easy.h>
#include <cassert>
#include <chrono>
#include <queue>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;
  constexpr int nums = 1'000'000;

  auto t1 = chrono::steady_clock::now();
  queue<int> q;
  for (int i = 0; i < nums / 2; ++i) {
    q.push(1);
  }
  for (int i = 0; i < nums / 2; ++i) {
    q.pop();
  }
  auto t2 = chrono::steady_clock::now();
  auto d1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
  cout << d1 << "ms\n";

  mt19937 gen(random_device{}());
  uniform_int_distribution<> dist(1, 2);
  fc::QueueTwoStacks<int> Q;
  t1 = chrono::steady_clock::now();
  for (int i = 0; i < nums; i++) {
    switch (dist(gen)) {
    case 1:
      Q.enqueue(1);
      break;
    case 2:
      if (!Q.empty()) {
        Q.dequeue();
      }
      break;
    default:
      assert(false);
      break;
    }
  }
  t2 = chrono::steady_clock::now();
  d1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
  cout << d1 << "ms\n";
}
