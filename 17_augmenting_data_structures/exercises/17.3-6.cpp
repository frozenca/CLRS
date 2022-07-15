#include <17_augmenting_data_structures/inc/easy.h>
#include <iostream>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::Rectangle2d> rects;
  rects.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);
  rects.emplace_back(2.0f, 0.0f, 1.0f, 1.0f);
  rects.emplace_back(4.0f, 0.0f, 1.0f, 1.0f);
  rects.emplace_back(6.0f, 0.0f, 1.0f, 1.0f);
  cout << fc::overlap_exist(rects) << '\n';
  rects.emplace_back(0.2f, 0.2f, 0.5f, 0.5f);
  cout << fc::overlap_exist(rects) << '\n';
}
