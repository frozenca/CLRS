#include <10_elementary_data_structures/inc/easy.h>
#include <iomanip>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    fc::XORList<int> l;
    l.list_insert(1);
    l.list_insert(2);
    l.list_insert(3);
    l.list_insert(4);
    l.list_insert(5);
    cout << l << '\n';
    cout << boolalpha << bool(l.list_search(1)) << '\n';
    cout << boolalpha << bool(l.list_search(2)) << '\n';
    cout << boolalpha << bool(l.list_search(3)) << '\n';
    cout << boolalpha << bool(l.list_search(4)) << '\n';
    cout << boolalpha << bool(l.list_search(5)) << '\n';
    l.list_delete(2);
    cout << l << '\n';
    l.list_reverse();
    cout << l << '\n';
  }
}