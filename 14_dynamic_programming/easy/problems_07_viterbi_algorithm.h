#ifndef __CLRS4_VITERBI_H__
#define __CLRS4_VITERBI_H__

#include <cassert>
#include <common.h>
#include <graph.h>
#include <string>
#include <string_view>
#include <vector>

namespace frozenca {

using namespace std;

struct Sound {
  string sound_;
  float prob_ = 0.0f;
  Sound() = default;
  Sound(string sound, float prob) : sound_{move(sound)}, prob_{prob} {}
};

using SoundGraph = DirGraph<int>;
using SequenceIter = vector<string_view>::const_iterator;

bool viterbi_get_path(DirGraph<int> &g, const DirEdgeProperty<int, Sound> &sound,
                      SequenceIter s_first, SequenceIter s_last, int curr_index,
                      vector<int> &succ) {
  if (s_first == s_last) {
    return true;
  }
  for (const auto &next_index : g.adj(curr_index)) {
    const auto &curr_sound = sound[{curr_index, next_index}];
    if (curr_sound.sound_ == *s_first) {
      auto res =
          viterbi_get_path(g, sound, next(s_first), s_last, next_index, succ);
      if (res) {
        succ[curr_index] = next_index;
        return true;
      }
    }
  }
  return false;
}

vector<int> viterbi_path(DirGraph<int> &g, int src,
                         const vector<string_view> &target_sequence) {

  const auto &sound = g.get_edge_property<Sound>(GraphPropertyTag::EdgeWeight);
  auto &succ = g.add_graph_property<vector<int>>(GraphPropertyTag::GraphPath);
  succ.resize(g.size(), -1);
  auto res = viterbi_get_path(g, sound, target_sequence.begin(),
                              target_sequence.end(), src, succ);
  if (!res) {
    return {};
  }
  vector<int> path;
  path.push_back(src);
  while (succ[src] != -1) {
    path.push_back(succ[src]);
    src = succ[src];
  }
  return path;
}

float viterbi_get_optimal_path(DirGraph<int> &g,
                               const DirEdgeProperty<int, Sound> &sound,
                               SequenceIter s_first, SequenceIter s_last,
                               int curr_index) {
  if (s_first == s_last) {
    g.get_graph_property<vector<int>>(GraphPropertyTag::GraphPath) =
        vector<int>({curr_index});
    return 1.0f;
  }
  vector<int> seq;
  float prob = 0.0f;
  for (const auto &next_index : g.adj(curr_index)) {
    const auto &curr_sound = sound[{curr_index, next_index}];
    if (curr_sound.sound_ == *s_first) {
      auto res_prob =
          viterbi_get_optimal_path(g, sound, next(s_first), s_last, next_index);
      if (res_prob > 0.0f && curr_sound.prob_ * res_prob > prob) {
        prob = curr_sound.prob_ * res_prob;
        swap(g.get_graph_property<vector<int>>(GraphPropertyTag::GraphPath),
             seq);
        seq.push_back(curr_index);
      }
    }
  }
  swap(g.get_graph_property<vector<int>>(GraphPropertyTag::GraphPath), seq);
  return prob;
}

pair<vector<int>, float>
viterbi_optimal_path(DirGraph<int> &g, int src,
                     const vector<string_view> &target_sequence) {
  const auto &sound = g.get_edge_property<Sound>(GraphPropertyTag::EdgeWeight);
  g.add_graph_property<vector<int>>(GraphPropertyTag::GraphPath).clear();
  auto prob = viterbi_get_optimal_path(g, sound, target_sequence.begin(),
                                       target_sequence.end(), src);
  auto seq = g.get_graph_property<vector<int>>(GraphPropertyTag::GraphPath);
  ranges::reverse(seq);
  return {move(seq), prob};
}

} // namespace frozenca

#endif //__CLRS4_VITERBI_H__
