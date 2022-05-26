#include <linkedlist_naive.h>
#include <iostream>
#include <iomanip>

namespace fc = frozenca;
using namespace std;

int main() {

  {
    fc::LinkedListSentinel<int> l1;
    l1.list_prepend(1);
    l1.list_prepend(2);
    l1.list_prepend(3);
    l1.list_prepend(4);
    l1.list_prepend(5);

    fc::LinkedListSentinel<int> l2;
    l2.list_prepend(6);
    l2.list_prepend(7);
    l2.list_prepend(8);
    l2.list_prepend(9);
    l2.list_prepend(10);

    auto l = fc::list_union(l1, l2);
    cout << l << '\n';
  }

}