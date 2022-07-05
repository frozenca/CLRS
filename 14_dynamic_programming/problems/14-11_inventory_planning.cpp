#include <14_dynamic_programming/inc/easy.h>
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  mt19937 gen(random_device{}());
  uniform_int_distribution<fc::index_t> demand_dist(0, 20);
  uniform_real_distribution<float> inventory_cost_dist(20.0f, 30.0f);

  constexpr int num_months = 12;

  vector<fc::index_t> demands;
  for (int i = 0; i < num_months; ++i) {
    demands.push_back(demand_dist(gen));
  }

  fc::index_t total_demand = accumulate(demands.begin(), demands.end(), 0ll);

  cout << "Total demand: " << total_demand << '\n';

  vector<float> inventory_cost(total_demand + 1);
  for (fc::index_t i = 1; i <= total_demand; ++i) {
    inventory_cost[i] = inventory_cost[i - 1] + inventory_cost_dist(gen);
  }
  cout << "Max inventory cost: " << inventory_cost.back() << '\n';

  constexpr float part_time_cost = 100.0f;
  constexpr fc::index_t regular_manufacture = 10ll;

  auto [min_cost, supply_plan] = fc::inventory_planning(
      demands, part_time_cost, regular_manufacture, inventory_cost);

  cout << "Min cost: " << min_cost << '\n';
  cout << "Supply plan: " << '\n';
  for (auto supply : supply_plan) {
    cout << supply << ' ';
  }
  cout << '\n';
}