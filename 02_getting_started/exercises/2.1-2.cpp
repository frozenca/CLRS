#include <02_getting_started/inc/easy.h>
#include <02_getting_started/inc/hard.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<int> v{2, 3, 1, 6, 5, 4};
    cout << fc::sum_array(v) << '\n';
  }
}