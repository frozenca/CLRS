#include <10_elementary_data_structures/inc/easy.h>
#include <common.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::TwoStacks<int, 10> ts;
  ts.push1(1);
  ts.push1(2);
  ts.push1(3);
  ts.push1(4);
  ts.push1(5);
  ts.push2(6);
  ts.push2(7);
  ts.push2(8);
  ts.push2(9);
  ts.push2(10);
  cout << ts.pop1() << '\n';
  cout << ts.pop1() << '\n';
  cout << ts.pop1() << '\n';
  cout << ts.pop1() << '\n';
  cout << ts.pop1() << '\n';
  cout << ts.pop2() << '\n';
  cout << ts.pop2() << '\n';
  cout << ts.pop2() << '\n';
  cout << ts.pop2() << '\n';
  cout << ts.pop2() << '\n';
}