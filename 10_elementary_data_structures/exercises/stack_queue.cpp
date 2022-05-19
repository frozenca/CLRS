#include <stack.h>
#include <queue.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  cout << "Stack\n";
  fc::Stack<int> s;
  s.push(15);
  s.push(6);
  s.push(2);
  s.push(9);
  cout << s.pop() << '\n';
  cout << s.pop() << '\n';
  cout << s.pop() << '\n';
  cout << s.pop() << '\n';

  cout << "Queue\n";
  fc::Queue<int> q;
  q.enqueue(15);
  q.enqueue(6);
  q.enqueue(2);
  q.enqueue(9);
  cout << q.dequeue() << '\n';
  cout << q.dequeue() << '\n';
  cout << q.dequeue() << '\n';
  cout << q.dequeue() << '\n';

}