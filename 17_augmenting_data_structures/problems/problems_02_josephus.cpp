#include <17_augmenting_data_structures/inc/easy.h>
#include <iostream>
#include <test.h>
#include <util.h>

int main() {
  namespace fc = frozenca;
  using namespace std;

  auto v1 = fc::josephus<3>(7);
  fc::print(v1);

  auto v2 = fc::josephus(7, 3);
  fc::print(v2);
}
