#include <14_dynamic_programming/inc/easy.h>
#include <cassert>
#include <graph.h>
#include <iostream>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace fc = frozenca;
using namespace std;

string generate_random_abcde_string() {
  mt19937 gen(random_device{}());
  uniform_int_distribution<int> length_dist(1, 1);
  uniform_int_distribution<int> ch_dist(97, 101);

  int len = length_dist(gen);
  string res;
  for (int l = 0; l < len; ++l) {
    res += static_cast<char>(ch_dist(gen));
  }
  return res;
}

int main() {
  fc::SoundGraph g;
  mt19937 gen(random_device{}());
  uniform_int_distribution<int> index_dist(0, 100);
  uniform_real_distribution<float> trans_prob_dist(0.0f, 1.0f);
  for (int i = 0; i < 30'000; ++i) {
    int src = index_dist(gen);
    int dst = index_dist(gen);
    if (src == dst) {
      continue;
    } else if (src > dst) {
      swap(src, dst);
    }
    g.add_edge(src, dst);
    g(fc::e_w, {src, dst}) = {generate_random_abcde_string(),
                              trans_prob_dist(gen)};
  }
  // normalize transition probs
  for (int i = 0; i < g.size(); ++i) {
    float prob_sum = 0.0f;
    for (const auto &e : g.adj(i)) {
      prob_sum += g(fc::e_w, e).prob_;
    }
    for (auto &e : g.adj(i)) {
      g(fc::e_w, e).prob_ /= prob_sum;
    }
  }

  // construct target sound
  vector<string_view> target_sound_sequence;
  {
    int curr = 0;
    while (!g.adj(curr).empty()) {
      const auto &next_edge = *g.adj(curr).begin();
      target_sound_sequence.emplace_back(g(fc::e_w, next_edge).sound_);
      curr = next_edge.second;
    }
  }

  cout << "Target sound:\n";
  for (auto target_sound : target_sound_sequence) {
    cout << target_sound;
  }
  cout << '\n';

  {
    cout << "Path:\n";
    auto path = fc::viterbi_path(g, 0, target_sound_sequence);
    assert(!path.empty());
    float prob = 1.0f;
    for (int i = 0; i < ssize(path) - 1; ++i) {
      const auto &sound = g(fc::e_w, {path[i], path[i + 1]});
      cout << path[i] << "->" << path[i + 1] << '(' << sound.sound_ << ")\n";
      prob *= sound.prob_;
    }
    cout << "Path probability " << prob << '\n';
  }

  {
    cout << "Optimal path:\n";
    auto [path, prob] = fc::viterbi_optimal_path(g, 0, target_sound_sequence);
    assert(!path.empty());
    for (int i = 0; i < ssize(path) - 1; ++i) {
      const auto &sound = g(fc::e_w, {path[i], path[i + 1]});
      cout << path[i] << "->" << path[i + 1] << '(' << sound.sound_ << ")\n";
    }
    cout << "Path probability " << prob << '\n';
  }
}
