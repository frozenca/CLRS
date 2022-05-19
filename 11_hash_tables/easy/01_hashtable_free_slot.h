#ifndef __CLRS4_HASH_TABLE_FREE_SLOT_H__
#define __CLRS4_HASH_TABLE_FREE_SLOT_H__

#include <hashtable.h>

namespace frozenca {

using namespace std;

template <typename K, typename V> class FlagNode {
public:
  K key_;
  V value_;
  bool free_ = true;
  FlagNode *prev_ = nullptr;
  FlagNode *next_ = nullptr;
  FlagNode(const pair<K, V> &x) : key_{x.first}, value_{x.second} {}
};

template <typename K, typename V, size_t N = arr_default_length,
          typename H = HashFunctionDefault<K, N>>
requires Hashable<H, K>
class HashTableFreeSlot {
  using Node = FlagNode<K, V>;
  using ListNown = LinkedPairList<K, V, Node, false, false>;
  using ListOwn = LinkedPairList<K, V, Node, true, false>;
  H h = H{};
  array<ListNown, N> T = {};
  ListOwn free_list = {};

public:
  HashTableFreeSlot() {
    for (size_t i = 0; i < N; ++i) {
      Node *node = new Node({});
      free_list.list_prepend(node);
      T[i].list_prepend(node);
    }
  }

  void chained_hash_insert(const pair<K, V> &x) {
    auto slot = T[h(x.first)];
    auto head = slot.head();
    assert(head);
    if (head->free_) {
      free_list.list_delete(head);
      head->free_ = false;
      head->key_ = x.first;
      head->value_ = x.second;
    } else {
      auto free_head = free_list.head();
      free_list.list_delete(free_head);
      free_head->free_ = false;
      free_head->key_ = x.first;
      free_head->value_ = x.second;
      free_head->next_ = head;
      slot.list_prepend(free_head);
    }
  }

  V chained_hash_search(const K &key) const {
    return T[h(key)].list_search(key);
  }

  void chained_hash_delete(const K &key) {
    auto slot = T[h(key)];
    auto head = slot.head();
    assert(head);
    if (head->free_) {
      return;
    } else {
      auto node = slot.list_search_node(key);
      slot.list_delete(node);
      node->free_ = true;
      node->key_ = K{};
      node->value_ = V{};
      free_list.list_prepend(node);
    }
  }

  friend ostream &operator<<(ostream &os, const HashTableFreeSlot &h) {
    for (const auto &l : h.T) {
      if (l.head()->free_) {
        continue;  
      }
      os << l;
    }
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_HASH_TABLE_FREE_SLOT_H__
