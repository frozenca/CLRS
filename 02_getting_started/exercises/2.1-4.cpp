#include <02_getting_started/inc/easy.h>
#include <02_getting_started/inc/hard.h>
#include <iostream>
#include <vector>


int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> v{2, 3, 1, 6, 5, 4};
    cout << fc::linear_search(v, 6) << '\n';
  }
  {
    vector<int> v{2, 3, 1, 6, 5, 4};
    cout << ranges::distance(ranges::begin(v), ranges::find(v, 6)) << '\n';
  }
}