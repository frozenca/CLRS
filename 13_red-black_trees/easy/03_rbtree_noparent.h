#ifndef __CLRS4_RB_TREE_NOPARENT_H__
#define __CLRS4_RB_TREE_NOPARENT_H__

#include <cassert>
#include <iostream>
#include <rbtree.h>
#include <vector>

namespace frozenca {

using namespace std;

template <Containable K> class RBTreeNoParent : public hard::TreeSet<K> {
public:
  using Node = hard::TreeSet<K>::node_type;

  void insert_noparent(const K &key) {
    vector<Node *> nodes;
    auto z = make_unique<Node>(key);
    Node *y = nullptr;
    Node *x = this->get_root();
    while (x) {
      y = x;
      nodes.push_back(x);
      if (z->key_ < x->key_) {
        x = x->left_.get();
      } else {
        x = x->right_.get();
      }
    }
    Node *z_ptr = nullptr;
    if (!y) {
      this->set_root(move(z));
      z_ptr = this->get_root();
      insert_fixup_noparent(this->get_root(), nodes);
    } else if (z->key_ < y->key_) {
      y->left_ = move(z);
      z_ptr = y->left_.get();
      insert_fixup_noparent(y->left_.get(), nodes);
    } else {
      y->right_ = move(z);
      z_ptr = y->right_.get();
      insert_fixup_noparent(y->right_.get(), nodes);
    }
  }

  void left_rotate_noparent(Node *xp, unique_ptr<Node> x) {
    auto y = move(x->right_);
    x->right_ = move(y->left_);
    if (!xp) {
      auto px = x.release();
      this->set_root(move(y));
      this->get_root()->left_ = unique_ptr<Node>(px);
    } else if (x == xp->left_) {
      auto px = x.release();
      xp->left_ = move(y);
      xp->left_->left_ = unique_ptr<Node>(px);
    } else {
      auto px = x.release();
      xp->right_ = move(y);
      xp->right_->left_ = unique_ptr<Node>(px);
    }
  }

  void right_rotate_noparent(Node *xp, unique_ptr<Node> x) {
    auto y = move(x->left_);
    x->left_ = move(y->right_);
    if (!xp) {
      auto px = x.release();
      this->set_root(move(y));
      this->get_root()->right_ = unique_ptr<Node>(px);
    } else if (x == xp->left_) {
      auto px = x.release();
      xp->left_ = move(y);
      xp->left_->right_ = unique_ptr<Node>(px);
    } else {
      auto px = x.release();
      xp->right_ = move(y);
      xp->right_->right_ = unique_ptr<Node>(px);
    }
  }

  void insert_fixup_noparent(Node *z, vector<Node *> &nodes) {
    Node *zp = nullptr;
    if (!nodes.empty()) {
      zp = nodes.back();
    }
    while (zp && !zp->black_) {
      zp = nodes.back();
      nodes.pop_back();
      assert(!nodes.empty());
      auto zpp = nodes.back();
      if (zp == zpp->left_.get()) { // is z's parent a left child?
        auto y = zpp->right_.get(); // y is z's uncle
        if (y && !y->black_) {      // are z's parent and uncle both red?
          // case 1
          zp->black_ = true;
          y->black_ = true;
          zpp->black_ = false;
          nodes.pop_back();
        } else {
          // case 2
          if (z == zp->right_.get()) {
            left_rotate_noparent(zpp, move(zpp->left_));
            zp = zpp->left_.get();
          }
          // case 3
          zp->black_ = true;
          zpp->black_ = false;
          nodes.pop_back();
          auto zppp = nodes.empty() ? nullptr : nodes.back();
          nodes.push_back(zp);
          if (!zppp) {
            right_rotate_noparent(zppp, this->extract_root());
          } else if (zpp == zppp->left_.get()) {
            right_rotate_noparent(zppp, move(zppp->left_));
          } else {
            right_rotate_noparent(zppp, move(zppp->right_));
          }
        }
      } else { // same as the above, but with "right" and "left" exchanged
        auto y = zpp->left_.get(); // y is z's uncle
        if (y && !y->black_) {     // are z's parent and uncle both red?
          // case 1
          zp->black_ = true;
          y->black_ = true;
          zpp->black_ = false;
          nodes.pop_back();
        } else {
          // case 2
          if (z == zp->left_.get()) {
            right_rotate_noparent(zpp, move(zpp->right_));
            zp = zpp->right_.get();
          }
          // case 3
          zp->black_ = true;
          zpp->black_ = false;
          nodes.pop_back();
          auto zppp = nodes.empty() ? nullptr : nodes.back();
          nodes.push_back(zp);
          if (!zppp) {
            left_rotate_noparent(zppp, this->extract_root());
          } else if (zpp == zppp->left_.get()) {
            left_rotate_noparent(zppp, move(zppp->left_));
          } else {
            left_rotate_noparent(zppp, move(zppp->right_));
          }
        }
      }
    }
    this->get_root()->black_ = true;
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
