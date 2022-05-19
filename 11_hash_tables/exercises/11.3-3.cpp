#include <11_hash_tables/inc/easy.h>
#include <hashtable.h>
#include <iostream>
#include <string>
#include <vector>

namespace fc = frozenca;
using namespace std;

struct BadHashFunction {
  size_t operator()(const string &v) const {
    unsigned char key = 0;
    for (const auto &c : v) {
      auto n_repr = bit_cast<unsigned char>(c);
      key += n_repr;
    }
    return key;
  }
};

int main() {

  {
    fc::HashTable<string, int, BadHashFunction> h;
    h.chained_hash_insert({"abcd", 4});
    h.chained_hash_insert({"dcba", 6}); // collides
    cout << h << '\n';
  }
}