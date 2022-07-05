#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <string>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::edit_distance("algorithm", "altruistic");
  cout << '\n';
  fc::max_alignment("GATCGGCAT", "CAATGTGAATC");
}