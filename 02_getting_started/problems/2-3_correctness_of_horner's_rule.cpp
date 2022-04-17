#include <02_getting_started/inc/easy.h>
#include <02_getting_started/inc/hard.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    vector<float> coefficients {1.0f, -2.0f, 1.0f};
    cout << fc::horner(coefficients, 1.0f) << '\n';
    cout << fc::horner(coefficients, 2.0f) << '\n';
  }
}