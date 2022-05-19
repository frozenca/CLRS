#include <11_hash_tables/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::DirectAddressTable table;
  table.direct_address_insert({3, 6});
  table.direct_address_insert({7, 2});
  table.direct_address_insert({5, 8});
  table.direct_address_delete(5);
  cout << table.direct_address_search(3) << '\n';
  cout << table.direct_address_search(5) << '\n';
  cout << table.direct_address_search(7) << '\n';
  cout << table.maximum() << '\n';

}