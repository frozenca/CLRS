#ifndef __CLRS4_SCHEDULE_MINIIMIZE_AVG_COMPLETION_H__
#define __CLRS4_SCHEDULE_MINIIMIZE_AVG_COMPLETION_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <queue>
#include <tuple>
#include <vector>

namespace frozenca {

using namespace std;

pair<float, vector<index_t>>
minimize_average_completion(vector<index_t> &processing_times) {
  ranges::sort(processing_times);
  vector<index_t> tasks;
  index_t total_time = 0;
  index_t total_completion_time = 0;
  for (auto processing_time : processing_times) {
    tasks.push_back(processing_time);
    total_time += processing_time;
    total_completion_time += total_time;
  }
  auto avg_time =
      total_completion_time / static_cast<float>(ssize(processing_times));
  return {avg_time, move(tasks)};
}

pair<float, vector<index_t>> minimize_average_completion_release_time(
    const vector<pair<index_t, index_t>> &unordered_tasks) {
  // tasks are in (processing time, release_time)
  vector<tuple<index_t, index_t, index_t>> tasks;
  for (index_t i = 0; i < ssize(unordered_tasks); ++i) {
    const auto &[proc_time, rel_time] = unordered_tasks[i];
    tasks.emplace_back(rel_time, proc_time, i);
  }
  // tasks are in (release time, processing time, index) now
  ranges::sort(tasks);
  vector<index_t> ordered_tasks;
  index_t total_time = 0;
  index_t total_completion_time = 0;

  auto start_time = get<0>(tasks.front());
  priority_queue<pair<index_t, index_t>, vector<pair<index_t, index_t>>,
                 greater<>>
      remaining;
  for (const auto &[rel_time, proc_time, idx] : tasks) {
    remaining.emplace(proc_time, idx);
  }
  for (index_t t = start_time; !remaining.empty(); ++t) {
    auto [curr_proc_time, curr_index] = remaining.top();
    // process a task
    curr_proc_time--;
    ordered_tasks.push_back(curr_index);
    remaining.pop();
    if (curr_proc_time) {
      remaining.emplace(curr_proc_time, curr_index);
    } else {
      total_time += (t - start_time);
      total_completion_time += total_time;
    }
  }

  auto avg_time =
      total_completion_time / static_cast<float>(ssize(unordered_tasks));
  return {avg_time, move(ordered_tasks)};
}

} // namespace frozenca

#endif //__CLRS4_SCHEDULE_MINIIMIZE_AVG_COMPLETION_H__
