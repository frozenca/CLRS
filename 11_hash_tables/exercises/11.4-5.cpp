#include <11_hash_tables/inc/easy.h>
#include <hashtable_naive.h>
#include <iostream>
#include <string>
#include <vector>

namespace fc = frozenca;
using namespace std;

int main() {
  
  {
    fc::HashTableOpenAddressing<fc::HashFunctionDouble<>> h;
    h.hash_insert(1);
    h.hash_insert(3);
    h.hash_insert(6);
    cout << h << '\n';
    cout << h.hash_search_examine(2) << '\n'; // examines the whole table!
  }
}