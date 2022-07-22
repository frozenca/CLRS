#include <algorithm>
#include <btree.h>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  fc::BTreeSet<int> btree1;
  for (int i = 0; i < 500'000; ++i) {
    btree1.insert(i);
  }
  fc::BTreeSet<int> btree2;
  for (int i = 500'001; i < 1'000'000; ++i) {
    btree2.insert(i);
  }

  cout << "For B-trees with 500000 + 500000 elements:\n";

  auto start = chrono::steady_clock::now();
  auto btree3 = fc::detail::join(move(btree1), 500'000, move(btree2));
  auto end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::duration<float, micro>>(end - start)
          .count();

  cout << "Join " << duration << "us\n";

  start = chrono::steady_clock::now();
  auto [btree4, btree5] = fc::detail::split(move(btree3), 500'000);
  end = chrono::steady_clock::now();
  duration = chrono::duration_cast<chrono::duration<float, micro>>(end - start)
                 .count();

  cout << "Split " << duration << "us\n";
}
