#include <14_dynamic_programming/inc/easy.h>
#include <iostream>
#include <memory>

int main() {
  namespace fc = frozenca;
  using namespace std;

  auto CEO = make_unique<fc::Employee>(-2.0f);
  CEO->first_subordinate_ = make_unique<fc::Employee>(2.0f);
  CEO->first_subordinate_->next_coworker_ = make_unique<fc::Employee>(3.0f);
  CEO->first_subordinate_->first_subordinate_ = make_unique<fc::Employee>(4.0f);

  cout << fc::maximum_conviviality(CEO.get());
}