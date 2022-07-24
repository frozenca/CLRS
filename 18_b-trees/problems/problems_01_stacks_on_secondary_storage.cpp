#include <18_b-trees/inc/easy.h>
#include <iostream>
#include <random>

int main() {
  namespace fc = frozenca;
  using namespace std;
  constexpr int num = 10'000;

  mt19937 gen(random_device{}());
  bernoulli_distribution to_push_or_pop(0.65);

  fc::DiskStack ds("diskstack.bin", 1ul << 24ul, true);

  for (int i = 0; i < num; ++i) {
    if (to_push_or_pop(gen)) {
      ds.push(1);
    } else {
      if (!ds.empty()) {
        ds.pop();
      }
    }
  }

  cout << "Page changes for " << num << " elements: " << ds.page_changes()
       << '\n';
}
