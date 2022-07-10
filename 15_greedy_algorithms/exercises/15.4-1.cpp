#include <15_greedy_algorithms/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<char> requests{'s', 'q', 's', 'q', 'q', 's', 'p', 'p',
                        'r', 's', 's', 'q', 'p', 'r', 'q'};

  cout << fc::offline_caching(requests, 3) << '\n';

  fc::cache_t init = {0};
  fc::offline_caching_furthest_in_future(init, requests, 3);
}
