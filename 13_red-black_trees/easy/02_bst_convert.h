#ifndef __CLRS4_BST_CONVERT_H__
#define __CLRS4_BST_CONVERT_H__

#include <cassert>
#include <charconv>
#include <iostream>
#include <rbtree.h>
#include <string>
#include <tuple>
#include <vector>


namespace frozenca {

using namespace std;

template <Containable K> class RBTreeConvert : public TreeSet<K> {
public:
  using Node = TreeSet<K>::node_type;
  [[nodiscard]] bool is_right_spine() const {
    ptrdiff_t spine_length = 0;
    auto curr = this->get_root();
    while (curr) {
      curr = curr->right_.get();
      ++spine_length;
    }
    return spine_length == this->size();
  }

  pair<ptrdiff_t, Node *> get_rotation_point() const {
    ptrdiff_t index = 0;
    const Node *curr = this->get_root();
    while (curr) {
      if (curr->left_) {
        return {index, const_cast<Node *>(curr)};
      }
      curr = curr->right_.get();
      ++index;
    }
    return {-1, nullptr};
  }

  vector<string> get_rotation_sequence() {
    vector<string> result;
    while (!is_right_spine()) {
      auto [rot, where] = get_rotation_point();
      assert(rot != -1 && where);
      result.push_back("R" + to_string(rot));
      this->right_rotate(where);
    }
    return result;
  }

  static vector<string>
  get_inverse_sequence(const vector<string> &rot_sequence) {
    vector<string> result;
    for (index_t i = 0; i < ssize(rot_sequence); ++i) {
      const auto &rot = rot_sequence[ssize(rot_sequence) - 1 - i];
      assert(rot.length() >= 2);
      string inv_rot = rot;
      if (rot[0] == 'L') {
        inv_rot[0] = 'R';
      } else if (rot[0] == 'R') {
        inv_rot[0] = 'L';
      } else {
        assert(0);
      }
      result.push_back(inv_rot);
    }
    return result;
  }

  vector<string> get_rotation_sequence(RBTreeConvert &other) {
    auto seq1 = get_rotation_sequence();
    auto seq2 =
        RBTreeConvert::get_inverse_sequence(other.get_rotation_sequence());
    ranges::move(seq2, back_inserter(seq1));
    return seq1;
  }

  [[nodiscard]] tuple<bool, Node *, ptrdiff_t>
  in_the_left_spine(const Node *this_root, const Node *other_root) const {
    auto curr = this_root;
    ptrdiff_t offset = 0;
    while (curr) {
      if (curr->key_ == other_root->key_) {
        return {true, const_cast<Node *>(curr), offset};
      }
      curr = curr->left_.get();
      ++offset;
    }
    return {false, nullptr, -1};
  }

  vector<string> get_right_rotation_sequence(RBTreeConvert &other) {
    if (this->size() != other.size()) {
      return {"IMPOSSIBLE"};
    }
    vector<string> result;
    get_right_rotation_sequence(this->get_root(), other.get_root(), "", result);
    return result;
  }

  void get_right_rotation_sequence(Node *this_root, const Node *other_root,
                                   string prefix, vector<string> &result) {
    if (result.size() == 1 && result.back() == "IMPOSSIBLE") {
      return;
    } else if (!this_root && !other_root) {
      return;
    } else if ((this_root && !other_root) || (!this_root && other_root)) {
      result.clear();
      result.push_back("IMPOSSIBLE");
      return;
    }
    auto [to_rotate, where, offset] = in_the_left_spine(this_root, other_root);
    if (to_rotate) {
      while (where->parent_ && where != this_root) {
        --offset;
        this->right_rotate(where->parent_);
        result.push_back(prefix + to_string(offset));
      }
      get_right_rotation_sequence(where->left_.get(), other_root->left_.get(),
                                  prefix + "L", result);
      get_right_rotation_sequence(where->right_.get(), other_root->right_.get(),
                                  prefix + "R", result);
    } else {
      result.clear();
      result.push_back("IMPOSSIBLE");
      return;
    }
  }

  void print(const Node *node) const {
    if (node) {
      cout << node->key_ << ' ';
      print(node->left_.get());
      print(node->right_.get());
    } else {
      cout << "# ";
    }
  }

  void print() const { print(this->get_root()); }
};

} // namespace frozenca

#endif //__CLRS4_BST_CONVERT_H__
