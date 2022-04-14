#include <02_getting_started/inc/easy.h>
#include <02_getting_started/inc/hard.h>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> v{1, 2, 4, 6, 7};
    cout << boolalpha << fc::two_sum(v, 11) << '\n';
  }
  {
    vector<int> v{1, 2, 4, 6, 7};
    cout << boolalpha << fc::two_sum(v, 20) << '\n';
  }
  {
    vector<int> v{1, 2, 4, 6, 7};
    cout << boolalpha << fc::two_sum(v, -2) << '\n';
  }
  
}