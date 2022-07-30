#include <19_data_structures_for_disjoint_sets/inc/easy.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  vector<fc::ListElem<char> *> ve;

  for (char c = 'a'; c <= 'j'; ++c) {
    ve.push_back(fc::make_listset(c));
  }

  fc::union_listset(ve[1], ve[3]);
  fc::union_listset(ve[4], ve[5]);
  fc::union_listset(ve[0], ve[2]);
  fc::union_listset(ve[7], ve[8]);
  fc::union_listset(ve[0], ve[1]);
  fc::union_listset(ve[5], ve[6]);
  fc::union_listset(ve[1], ve[2]);

  for (const auto &ls : ve) {
    cout << ls->elem_ << ' ' << ls->head_->sz_ << ' ' << ls->head_->elem_
         << '\n';
  }

  for (auto p : ve) {
    delete p;
  }
}
