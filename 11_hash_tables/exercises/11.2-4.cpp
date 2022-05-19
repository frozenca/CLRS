#include <11_hash_tables/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::HashTableFreeSlot<int, int> h;
  h.chained_hash_insert({3, 4});
  h.chained_hash_insert({5, 6});
  h.chained_hash_insert({7, 8});
  cout << h << '\n';
  cout << h.chained_hash_search(3) << '\n';
  h.chained_hash_delete(5);
  cout << h << '\n';


}