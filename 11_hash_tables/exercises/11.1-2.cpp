#include <11_hash_tables/inc/easy.h>
#include <iostream>
#include <iomanip>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::BitVector table;
  table.direct_address_insert(3);
  table.direct_address_insert(5);
  table.direct_address_insert(7);
  table.direct_address_delete(5);
  cout << boolalpha << table.direct_address_search(3) << '\n';
  cout << boolalpha << table.direct_address_search(5) << '\n';
  cout << boolalpha << table.direct_address_search(7) << '\n';

}