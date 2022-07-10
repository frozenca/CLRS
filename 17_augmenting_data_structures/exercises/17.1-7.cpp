#include <02_getting_started/inc/easy.h>
#include <17_augmenting_data_structures/inc/easy.h>
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> v{2, 3, 8, 6, 1};
    cout << fc::count_inversion(v) << '\n';
  }
  {
    vector<int> v{2, 3, 8, 6, 1};
    cout << fc::inversions_tree(v) << '\n';
  }
}