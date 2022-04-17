#include <06_heapsort/inc/easy.h>
#include <algorithm>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    fc::PQQueue q;
    for (int i = 0; i < 5; ++i) {
      q.enqueue(i);
    }
    for (int i = 0; i < 5; ++i) {
      cout << q.dequeue() << ' ';
    }
    cout << '\n';
  }
  {
    fc::PQStack q;
    for (int i = 0; i < 5; ++i) {
      q.push(i);
    }
    for (int i = 0; i < 5; ++i) {
      cout << q.pop() << ' ';
    }
    cout << '\n';
  }
}