#include <iomanip>
#include <iostream>
#include <linkedlist_naive.h>


int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    cout << "Linked list\n";
    fc::LinkedList<int> l;
    l.list_prepend(1);
    l.list_prepend(2);
    l.list_prepend(3);
    l.list_prepend(4);
    l.list_prepend(5);
    cout << l << '\n';
    cout << boolalpha << bool(l.list_search(1)) << '\n';
    cout << boolalpha << bool(l.list_search(2)) << '\n';
    cout << boolalpha << bool(l.list_search(3)) << '\n';
    cout << boolalpha << bool(l.list_search(4)) << '\n';
    cout << boolalpha << bool(l.list_search(5)) << '\n';
    auto pt3 = l.list_search(3);
    l.list_insert(pt3, 6);
    cout << l << '\n';
    l.list_delete(pt3);
    cout << l << '\n';
  }

  {
    cout << "Linked list with sentinel\n";
    fc::LinkedListSentinel<int> l;
    l.list_prepend(1);
    l.list_prepend(2);
    l.list_prepend(3);
    l.list_prepend(4);
    l.list_prepend(5);
    cout << l << '\n';
    cout << boolalpha << bool(l.list_search(1)) << '\n';
    cout << boolalpha << bool(l.list_search(2)) << '\n';
    cout << boolalpha << bool(l.list_search(3)) << '\n';
    cout << boolalpha << bool(l.list_search(4)) << '\n';
    cout << boolalpha << bool(l.list_search(5)) << '\n';
    auto pt3 = l.list_search(3);
    l.list_insert(pt3, 6);
    cout << l << '\n';
    l.list_delete(pt3);
    cout << l << '\n';
  }
}