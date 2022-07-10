#ifndef __CLRS4_HUFFMAN_H__
#define __CLRS4_HUFFMAN_H__

#include <cassert>
#include <common.h>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

namespace frozenca {

using namespace std;

struct HuffmanNode {
  char value_ = 0;
  index_t freq_ = 0;
  unique_ptr<HuffmanNode> left_;
  unique_ptr<HuffmanNode> right_;
  HuffmanNode() = default;
  HuffmanNode(char value, index_t freq) : value_{value}, freq_{freq} {}

  HuffmanNode(const HuffmanNode &node) = delete;
  HuffmanNode &operator=(const HuffmanNode &node) = delete;
  HuffmanNode(HuffmanNode &&node) = delete;
  HuffmanNode &operator=(HuffmanNode &&node) = delete;
};

unique_ptr<HuffmanNode> huffman(const vector<pair<char, index_t>> &char_freq) {
  auto huffman_comp = [](const auto &cf1, const auto &cf2) {
    return cf1->freq_ > cf2->freq_;
  };
  vector<unique_ptr<HuffmanNode>> Q;
  for (const auto &[val, freq] : char_freq) {
    Q.emplace_back(make_unique<HuffmanNode>(val, freq));
  }
  ranges::make_heap(Q, huffman_comp);
  const auto n = ssize(char_freq);

  for (index_t i = 0; i < n - 1; ++i) {
    ranges::pop_heap(Q, huffman_comp);
    auto x = move(Q.back());
    Q.pop_back();
    ranges::pop_heap(Q, huffman_comp);
    auto y = move(Q.back());
    Q.pop_back();
    auto z = make_unique<HuffmanNode>('\0', x->freq_ + y->freq_);
    z->left_ = move(x);
    z->right_ = move(y);
    Q.push_back(move(z));
    ranges::push_heap(Q, huffman_comp);
  }
  return move(Q.back());
}

string huffman_decode(const HuffmanNode *root, string_view codeword) {
  if (!root || codeword.empty()) {
    return "";
  }
  string result;
  const auto n = ssize(codeword);
  index_t i = 0;
  while (i < n) {
    auto curr = root;
    while (i < n) {
      if (codeword[i] == '0') {
        if (curr->left_) {
          curr = curr->left_.get();
          ++i;
        } else {
          break;
        }
      } else if (codeword[i] == '1') {
        if (curr->right_) {
          curr = curr->right_.get();
          ++i;
        } else {
          break;
        }
      } else {
        assert(false);
      }
    }
    result += curr->value_;
  }
  return result;
}

struct HuffmanNodeTernary {
  char value_ = 0;
  index_t freq_ = 0;
  unique_ptr<HuffmanNodeTernary> left_;
  unique_ptr<HuffmanNodeTernary> mid_;
  unique_ptr<HuffmanNodeTernary> right_;
  HuffmanNodeTernary() = default;
  HuffmanNodeTernary(char value, index_t freq) : value_{value}, freq_{freq} {}

  HuffmanNodeTernary(const HuffmanNodeTernary &node) = delete;
  HuffmanNodeTernary &operator=(const HuffmanNodeTernary &node) = delete;
  HuffmanNodeTernary(HuffmanNodeTernary &&node) = delete;
  HuffmanNodeTernary &operator=(HuffmanNodeTernary &&node) = delete;
};

unique_ptr<HuffmanNodeTernary>
huffman_ternary(const vector<pair<char, index_t>> &char_freq) {
  auto huffman_ternary_comp = [](const auto &cf1, const auto &cf2) {
    return cf1->freq_ > cf2->freq_;
  };
  vector<unique_ptr<HuffmanNodeTernary>> Q;
  for (const auto &[val, freq] : char_freq) {
    Q.emplace_back(make_unique<HuffmanNodeTernary>(val, freq));
  }
  ranges::make_heap(Q, huffman_ternary_comp);

  while (ssize(Q) > 1) {
    ranges::pop_heap(Q, huffman_ternary_comp);
    auto x = move(Q.back());
    Q.pop_back();
    ranges::pop_heap(Q, huffman_ternary_comp);
    auto y = move(Q.back());
    Q.pop_back();
    unique_ptr<HuffmanNodeTernary> z;
    if (!Q.empty()) {
      ranges::pop_heap(Q, huffman_ternary_comp);
      z = move(Q.back());
      Q.pop_back();
    }
    auto w = make_unique<HuffmanNodeTernary>('\0', x->freq_ + y->freq_ + (z ? z->freq_ : 0));
    w->left_ = move(x);
    w->mid_ = move(y);
    w->right_ = move(z);
    Q.push_back(move(w));
    ranges::push_heap(Q, huffman_ternary_comp);
  }
  return move(Q.back());
}

string huffman_ternary_decode(const HuffmanNodeTernary *root,
                              string_view codeword) {
  if (!root || codeword.empty()) {
    return "";
  }
  string result;
  const auto n = ssize(codeword);
  index_t i = 0;
  while (i < n) {
    auto curr = root;
    while (i < n) {
      if (codeword[i] == '0') {
        if (curr->left_) {
          curr = curr->left_.get();
          ++i;
        } else {
          break;
        }
      } else if (codeword[i] == '1') {
        if (curr->mid_) {
          curr = curr->mid_.get();
          ++i;
        } else {
          break;
        }
      } else if (codeword[i] == '2') {
        if (curr->right_) {
          curr = curr->right_.get();
          ++i;
        } else {
          break;
        }
      } else {
        assert(false);
      }
    }
    result += curr->value_;
  }
  return result;
}

} // namespace frozenca

#endif //__CLRS4_HUFFMAN_H__
