#ifndef __CLRS4_INVENTORY_PLANNING_H__
#define __CLRS4_INVENTORY_PLANNING_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <ranges>
#include <vector>

namespace frozenca {

using namespace std;

pair<float, vector<index_t>>
inventory_planning(const vector<index_t> &demand, float part_time_cost,
                   index_t regular_manufacture,
                   const vector<float> &inventory_cost) {
  const auto months = ssize(demand);
  assert(months > 0);
  auto total_demand = accumulate(demand.begin(), demand.end(), 0ll);
  assert(ssize(inventory_cost) == total_demand + 1);
  vector<vector<float>> cost(
      months, vector<float>(total_demand + 1, numeric_limits<float>::max()));
  vector<vector<index_t>> to_supply(months, vector<index_t>(total_demand + 1));

  index_t future_demand = total_demand;
  // first month, start with zero machines
  {
    auto supply_min = demand[0];
    auto supply_max = future_demand;
    auto previous_cost = 0;
    for (index_t supply = supply_min; supply <= supply_max; ++supply) {
      auto manufacture_cost =
          part_time_cost * max(supply - regular_manufacture, 0ll);
      index_t leftover = supply - demand[0];
      auto leftover_cost = inventory_cost[leftover];
      auto curr_cost = previous_cost + manufacture_cost + leftover_cost;
      if (curr_cost < cost[0][leftover]) {
        cost[0][leftover] = curr_cost;
        to_supply[0][leftover] = supply;
      }
    }
    future_demand -= demand[0];
  }
  for (index_t curr_month = 1; curr_month < months; ++curr_month) {
    for (index_t curr_machines = 0; curr_machines <= total_demand;
         curr_machines++) {
      auto previous_cost = cost[curr_month - 1][curr_machines];
      if (previous_cost == numeric_limits<float>::max()) {
        continue;
      }
      auto supply_min = max(demand[curr_month] - curr_machines, 0ll);
      auto supply_max = max(future_demand - curr_machines, 0ll);
      for (index_t supply = supply_min; supply <= supply_max; ++supply) {
        auto manufacture_cost =
            part_time_cost * max(supply - regular_manufacture, 0ll);
        index_t leftover = curr_machines + supply - demand[curr_month];
        auto leftover_cost = inventory_cost[leftover];
        auto curr_cost = previous_cost + manufacture_cost + leftover_cost;
        if (curr_cost < cost[curr_month][leftover]) {
          cost[curr_month][leftover] = curr_cost;
          to_supply[curr_month][leftover] = supply;
        }
      }
    }
    future_demand -= demand[curr_month];
  }

  auto min_final_cost_iter = ranges::min_element(cost[months - 1]);
  auto leftover_index = distance(cost[months - 1].begin(), min_final_cost_iter);
  float min_final_cost = *min_final_cost_iter;
  auto final_supply = to_supply[months - 1][leftover_index];
  vector<index_t> supply_plan;
  supply_plan.push_back(final_supply);

  index_t curr_leftover = leftover_index - final_supply + demand[months - 1];

  for (index_t month = months - 2; month >= 0; --month) {
    auto curr_supply = to_supply[month][curr_leftover];
    supply_plan.push_back(curr_supply);
    index_t prev_leftover = curr_leftover - curr_supply + demand[month];
    curr_leftover = prev_leftover;
  }

  ranges::reverse(supply_plan);

  return {min_final_cost, move(supply_plan)};
}

} // namespace frozenca

#endif //__CLRS4_INVENTORY_PLANNING_H__
