#include <iomanip>
#include <iostream>
#include <linkedlist.h>


namespace fc = frozenca;
using namespace std;

int main() {

  fc::LinkedListSentinel<int> l;
  l.list_prepend(1);
  l.list_prepend(2);
  l.list_prepend(3);
  l.list_prepend(4);
  l.list_prepend(5);

  cout << l << '\n';

  l.list_reverse();

  cout << l << '\n';
}