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

using SoundGraph = WeightedDiGraph<Sound>;
using SequenceIter = vector<string_view>::const_iterator;

bool viterbi_get_path(const SoundGraph &g, SequenceIter s_first,
                      SequenceIter s_last, index_t curr_index,
                      vector<index_t> &succ) {
  if (s_first == s_last) {
    return true;
  }
  for (const auto &[next_index, curr_sound] : g.edges(curr_index)) {
    if (curr_sound.sound_ == *s_first) {
      auto res = viterbi_get_path(g, next(s_first), s_last, next_index, succ);
      if (res) {
        succ[curr_index] = next_index;
        return true;
      }
    }
  }
  return false;
}

vector<index_t> viterbi_path(const SoundGraph &g, index_t src,
                             const vector<string_view> &target_sequence) {
  vector<index_t> succ(g.size(), -1);
  auto res = viterbi_get_path(g, target_sequence.begin(), target_sequence.end(),
                              src, succ);
  if (!res) {
    return {};
  }
  vector<index_t> path;
  path.push_back(src);
  while (succ[src] != -1) {
    path.push_back(succ[src]);
    src = succ[src];
  }
  return path;
}

pair<vector<index_t>, float> viterbi_get_optimal_path(const SoundGraph &g,
                                                      SequenceIter s_first,
                                                      SequenceIter s_last,
                                                      index_t curr_index) {
  if (s_first == s_last) {
    return {vector<index_t>({curr_index}), 1.0f};
  }
  vector<index_t> seq;
  float prob = 0.0f;
  for (const auto &[next_index, curr_sound] : g.edges(curr_index)) {
    if (curr_sound.sound_ == *s_first) {
      auto [res_seq, res_prob] =
          viterbi_get_optimal_path(g, next(s_first), s_last, next_index);
      if (res_prob > 0.0f && curr_sound.prob_ * res_prob > prob) {
        prob = curr_sound.prob_ * res_prob;
        swap(seq, res_seq);
        seq.push_back(curr_index);
      }
    }
  }
  return {move(seq), prob};
}

pair<vector<index_t>, float>
viterbi_optimal_path(const SoundGraph &g, index_t src,
                     const vector<string_view> &target_sequence) {
  auto [seq, prob] = viterbi_get_optimal_path(g, target_sequence.begin(),
                                              target_sequence.end(), src);
  ranges::reverse(seq);
  return {move(seq), prob};
}

} // namespace frozenca

#endif //__CLRS4_VITERBI_H__
