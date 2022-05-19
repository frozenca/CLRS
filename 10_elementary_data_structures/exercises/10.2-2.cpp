#include <10_elementary_data_structures/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::StackSinglyLinkedList<int> s;
  s.push(15);
  s.push(6);
  s.push(2);
  s.push(9);
  cout << s.pop() << '\n';
  cout << s.pop() << '\n';
  cout << s.pop() << '\n';
  cout << s.pop() << '\n';
}