#include <10_elementary_data_structures/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::QueueSinglyLinkedList<int> q;
  q.enqueue(15);
  q.enqueue(6);
  q.enqueue(2);
  q.enqueue(9);
  cout << q.dequeue() << '\n';
  cout << q.dequeue() << '\n';
  q.enqueue(3);
  q.enqueue(7);
  cout << q.dequeue() << '\n';
  cout << q.dequeue() << '\n';
  cout << q.dequeue() << '\n';
  cout << q.dequeue() << '\n';
}