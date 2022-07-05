#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  string str =
      "The algorithm has found universal application in decoding the "
      "convolutional codes used in both CDMA and GSM digital cellular, dial-up "
      "modems, satellite, deep-space communications, and 802.11 wireless LANs.";

  fc::print_neatly(str, 30);
}