#include <11_hash_tables/inc/easy.h>
#include <hashtable_naive.h>
#include <iostream>
#include <string>
#include <vector>

namespace fc = frozenca;
using namespace std;

int main() {
  {
    fc::HashTableOpenAddressing h;
    h.hash_insert(1);
    h.hash_insert(3);
    h.hash_insert(6);
    cout << h << '\n';
    cout << h.hash_search(3) << '\n';
    cout << h.hash_search(6) << '\n';
    cout << h.hash_search(1) << '\n';
    cout << h << '\n';
    cout << h.hash_delete(3) << '\n';
    cout << h << '\n';
  }

  {
    fc::HashTableOpenAddressing<fc::HashFunctionDouble<>> h;
    h.hash_insert(1);
    h.hash_insert(3);
    h.hash_insert(6);
    cout << h << '\n';
    cout << h.hash_search(3) << '\n';
    cout << h.hash_search(6) << '\n';
    cout << h.hash_search(1) << '\n';
    cout << h << '\n';
    cout << h.hash_delete(3) << '\n';
    cout << h << '\n';
  }
}