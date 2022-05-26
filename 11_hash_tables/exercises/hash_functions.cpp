#include <11_hash_tables/inc/easy.h>
#include <hashtable_naive.h>
#include <iostream>


int main() {
  namespace fc = frozenca;
  using namespace std;
  {
    fc::HashTable<int, int, fc::HashFunctionMultiplyShift> h;
    h.chained_hash_insert({3, 4});
    h.chained_hash_insert({5, 6});
    h.chained_hash_insert({7, 8});
    cout << h << '\n';
    cout << h.chained_hash_search(3) << '\n';
    h.chained_hash_delete(5);
    cout << h << '\n';
  }

  {
    fc::HashTable<vector<int64_t>, int, fc::HashFunctionVector<int64_t>> h;
    h.chained_hash_insert({vector<int64_t>{1, 2, 3}, 4});
    h.chained_hash_insert({vector<int64_t>{4, 5}, 6});
    h.chained_hash_insert({vector<int64_t>{7}, 8});
    cout << h.chained_hash_search(vector<int64_t>{1, 2, 3}) << '\n';
    h.chained_hash_delete(vector<int64_t>{4, 5});
    cout << h.chained_hash_search(vector<int64_t>{4, 5}) << '\n';
  }

  {
    fc::HashTable<string, int, fc::HashFunctionString> h;
    h.chained_hash_insert({"Joe", 4});
    h.chained_hash_insert({"Donald", 6});
    h.chained_hash_insert({"Yoon", 8});
    cout << h << '\n';
    cout << h.chained_hash_search("Donald") << '\n';
    h.chained_hash_delete("Yoon");
    cout << h << '\n';
  }
}