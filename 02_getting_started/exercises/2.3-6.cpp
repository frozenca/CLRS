#include <02_getting_started/inc/easy.h>
#include <02_getting_started/inc/hard.h>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> v{1, 2, 3, 4, 5, 6};
    cout << boolalpha << fc::binary_search_func(v, 3) << '\n';
    cout << boolalpha << fc::binary_search_func(v, 7) << '\n';
  }

  {
    vector<int> v{1, 2, 3, 4, 5, 6};
    cout << boolalpha << ranges::binary_search(v, 3) << '\n';
    cout << boolalpha << ranges::binary_search(v, 7) << '\n';
    fc::verify_binary_search(ranges::binary_search);
  }
  
}