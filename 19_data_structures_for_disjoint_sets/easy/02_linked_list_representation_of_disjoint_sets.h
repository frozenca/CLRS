#ifndef __CLRS4_LIST_UNION_FIND_H__
#define __CLRS4_LIST_UNION_FIND_H__

#include <cassert>
#include <common.h>
#include <list>

namespace frozenca {

using namespace std;

template <typename T> struct ListElem {
  T elem_;
  size_t sz_ = 1;
  ListElem *head_ = nullptr;
  ListElem *next_ = nullptr;
};

template <typename T> ListElem<T> *make_listset(const T &key) {
  ListElem<T> *l = new ListElem<T>();
  l->elem_ = key;
  l->head_ = l;
  l->next_ = nullptr;
  return l;
}

template <typename T> ListElem<T> *find_listset(ListElem<T> *elm) {
  return elm->head_;
}

template <typename T>
ListElem<T> *union_listset(ListElem<T> *ls1, ListElem<T> *ls2) {
  if (ls1->head_ == ls2->head_) {
    return ls1;
  }
  if (ls1->head_->sz_ < ls2->head_->sz_) {
    swap(ls1, ls2);
  }
  auto p1 = ls1->head_;
  while (p1->next_) {
    p1 = p1->next_;
  }
  auto it = ls2->head_;
  ls1->head_->sz_ += it->sz_;
  p1->next_ = it;
  for (; it; it = it->next_) {
    it->head_ = ls1->head_;
  }

  return ls1;
}

} // namespace frozenca

#endif //__CLRS4_LIST_UNION_FIND_H__
