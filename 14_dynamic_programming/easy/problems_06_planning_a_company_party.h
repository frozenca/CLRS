#ifndef __CLRS4_COMPANY_PARTY_H__
#define __CLRS4_COMPANY_PARTY_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace frozenca {

using namespace std;

struct Employee {
  float conviviality_ = 0.0f;
  unique_ptr<Employee> first_subordinate_;
  unique_ptr<Employee> next_coworker_;

  Employee() = default;
  Employee(float conviviality) : conviviality_{conviviality} {}
};

float maximum_conviviality(
    const vector<const Employee *> &employees,
    const unordered_map<const Employee *, index_t> &index_map) {
  const auto n = ssize(employees);
  vector<float> inclusive(n);
  vector<float> exclusive(n);
  for (index_t i = n - 1; i >= 0; --i) {
    float inc = 0;
    float exc = 0;
    auto curr_sub = employees[i]->first_subordinate_.get();
    while (curr_sub) {
      exc += exclusive[index_map.at(curr_sub)];
      inc += inclusive[index_map.at(curr_sub)];
      curr_sub = curr_sub->next_coworker_.get();
    }
    exclusive[i] = inc;
    inclusive[i] = max(exc + employees[i]->conviviality_, inc);
  }
  return inclusive[0];
}

float maximum_conviviality(const Employee *CEO) {
  queue<const Employee *> q;
  unordered_map<const Employee *, index_t> index_map;
  vector<const Employee *> employees;
  if (CEO) {
    q.push(CEO);
  }
  index_t curr_index = 0;
  while (!q.empty()) {
    auto node = q.front();
    q.pop();
    employees.push_back(node);
    index_map[node] = curr_index++;
    auto curr = node->first_subordinate_.get();
    while (curr) {
      q.push(curr);
      curr = curr->next_coworker_.get();
    }
  }
  return maximum_conviviality(employees, index_map);
}

} // namespace frozenca

#endif //__CLRS4_COMPANY_PARTY_H__
