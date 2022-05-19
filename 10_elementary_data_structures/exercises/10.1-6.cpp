#include <10_elementary_data_structures/inc/easy.h>
#include <common.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::Dequeue<int, 10> dq;
  dq.enqueue_front(1);
  dq.enqueue_front(2);
  dq.enqueue_back(3);
  dq.enqueue_back(4);
  dq.enqueue_back(5);
  dq.enqueue_front(6);
  dq.enqueue_front(7);
  dq.enqueue_front(8);
  dq.enqueue_back(9);
  dq.enqueue_front(10);

  cout << dq.dequeue_front() << '\n';
  cout << dq.dequeue_back() << '\n';
  cout << dq.dequeue_back() << '\n';
  cout << dq.dequeue_front() << '\n';
  cout << dq.dequeue_front() << '\n';
  cout << dq.dequeue_front() << '\n';
  cout << dq.dequeue_back() << '\n';
  cout << dq.dequeue_front() << '\n';
  cout << dq.dequeue_back() << '\n';
  cout << dq.dequeue_front() << '\n';

}