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
  uniform_int_distribution<fc::index_t> index_dist(0, 100);
  uniform_real_distribution<float> trans_prob_dist(0.0f, 1.0f);
  for (int i = 0; i < 30'000; ++i) {
    fc::index_t src = index_dist(gen);
    fc::index_t dst = index_dist(gen);
    if (src == dst) {
      continue;
    } else if (src > dst) {
      swap(src, dst);
    }
    g.add_edge(src, dst, generate_random_abcde_string(), trans_prob_dist(gen));
  }
  // normalize transition probs
  for (fc::index_t i = 0; i < g.size(); ++i) {
    float prob_sum = 0.0f;
    for (const auto &[_, curr_sound] : g.edges(i)) {
      prob_sum += curr_sound.prob_;
    }
    for (auto &[_, curr_sound] : g.edges(i)) {
      curr_sound.prob_ /= prob_sum;
    }
  }

  // construct target sound
  vector<string_view> target_sound_sequence;
  {
    fc::index_t curr = 0;
    while (!g.edges(curr).empty()) {
      const auto &[next, edge_sound] = *g.edges(curr).begin();
      target_sound_sequence.emplace_back(edge_sound.sound_);
      curr = next;
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
    for (fc::index_t i = 0; i < ssize(path) - 1; ++i) {
      const auto &[_, sound] = *g.edge(path[i], path[i + 1]);
      cout << path[i] << "->" << path[i + 1] << '(' << sound.sound_ << ")\n";
      prob *= sound.prob_;
    }
    cout << "Path probability " << prob << '\n';
  }

  {
    cout << "Optimal path:\n";
    auto [path, prob] = fc::viterbi_optimal_path(g, 0, target_sound_sequence);
    assert(!path.empty());
    for (fc::index_t i = 0; i < ssize(path) - 1; ++i) {
      const auto &[_, sound] = *g.edge(path[i], path[i + 1]);
      cout << path[i] << "->" << path[i + 1] << '(' << sound.sound_ << ")\n";
    }
    cout << "Path probability " << prob << '\n';
  }
}
