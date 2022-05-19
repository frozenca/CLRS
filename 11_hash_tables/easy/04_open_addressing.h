#ifndef __CLRS4_OPEN_ADDRESSING_H__
#define __CLRS4_OPEN_ADDRESSING_H__

#include <algorithm>
#include <common.h>
#include <hashtable.h>
#include <limits>
#include <stdexcept>

namespace frozenca {

using namespace std;

template <typename H, typename K>
concept ProbeHashable = requires(H h, K k, size_t i) {
  { h(k, i) } -> convertible_to<size_t>;
};

template <size_t N = hashtable_default_length> struct HashFunctionProbeDefault {
  size_t operator()(size_t key, size_t i) const {
    return (i + hash<size_t>{}(key)) % N;
  }
};

template <typename H1 = HashFunctionDefault<size_t, hashtable_default_length>,
          typename H2 = HashFunctionDefault<size_t, hashtable_default_length>,
          size_t N = hashtable_default_length>
struct HashFunctionDouble {
  H1 h1 = {};
  H1 h2 = {};
  size_t m_ = 7919;
  size_t operator()(size_t key, size_t i) const {
    return (h1(key) + i * h2(key)) % m_;
  }
};

template <typename H = HashFunctionProbeDefault<hashtable_default_length>,
          size_t N = hashtable_default_length>
requires ProbeHashable<H, size_t>
class HashTableOpenAddressing {
  H h = H{};
  array<size_t, N> T = {};
  static constexpr size_t NIL = numeric_limits<size_t>::max();
  static constexpr size_t DELETED = NIL / 7919;

public:
  HashTableOpenAddressing() { ranges::fill(T, NIL); }

  size_t hash_insert(size_t k) {
    size_t i = 0;
    do {
      auto q = h(k, i);
      if (T[q] == NIL || T[q] == DELETED) {
        T[q] = k;
        return q;
      } else {
        ++i;
      }
    } while (i < N);
    throw runtime_error("hash table overflow");
  }

  size_t hash_search(size_t k) {
    size_t i = 0;
    do {
      auto q = h(k, i);
      if (T[q] == k) {
        return q;
      }
      ++i;
    } while (i < N);
    return NIL;
  }

  size_t hash_search_examine(size_t k) {
    size_t i = 0;
    do {
      auto q = h(k, i);
      if (T[q] == k) {
        cout << "Examined " << i << " times\n";
        return q;
      }
      ++i;
    } while (i < N);
    return NIL;
  }

  size_t hash_delete(size_t k) {
    size_t i = 0;
    do {
      auto q = h(k, i);
      if (T[q] == k) {
        T[q] = DELETED;
        return q;
      }
      ++i;
    } while (i < N);
    return NIL;
  }

  friend ostream &operator<<(ostream &os, const HashTableOpenAddressing &h) {
    for (const auto &l : h.T) {
      if (l != HashTableOpenAddressing::NIL && l != HashTableOpenAddressing::DELETED) {
        os << l << ' ';
      }
    }
    return os;
  }
};


template <typename H = HashFunctionProbeDefault<hashtable_default_length>,
          size_t N = hashtable_default_length>
requires ProbeHashable<H, size_t>
class HashTableOpenAddressingNonDelete {
  H h = H{};
  array<size_t, N> T = {};
  static constexpr size_t NIL = numeric_limits<size_t>::max();

public:
  HashTableOpenAddressingNonDelete() { ranges::fill(T, NIL); }

  size_t hash_insert(size_t k) {
    size_t i = 0;
    do {
      auto q = h(k, i);
      if (T[q] == NIL) {
        T[q] = k;
        return q;
      } else {
        ++i;
      }
    } while (i < N);
    throw runtime_error("hash table overflow");
  }

  size_t hash_search(size_t k) {
    size_t i = 0;
    do {
      auto q = h(k, i);
      if (T[q] == k) {
        return q;
      }
      ++i;
    } while (i < N);
    return NIL;
  }

  size_t hash_delete(size_t k) {
    size_t i = 0;
    do {
      auto q = h(k, i);
      if (T[q] == k) {
        hash_delete_linear_probing(q);
        return q;
      }
      ++i;
    } while (i < N);
    return NIL;
  }

private:
  size_t inverse_f(size_t k, size_t q) const {
      return (q - h(k, 0)) % N;
  }

  void hash_delete_linear_probing(size_t q) {
    while (true) {
      T[q] = NIL;  // make slot q empty
      auto q_next = q; // starting point for search
      size_t k_next = NIL;
      while (true) {
        q_next = (q_next + 1) % N; // next slot number with linear probing
        k_next = T[q_next]; // next key to try to move
        if (k_next == NIL) {
            return; // return when an empty slot is found
        }
        if (inverse_f(k_next, q) < inverse_f(k_next, q_next)) { // was empty slot q probed before q_next?
            break;
        }
      }
      T[q] = k_next; // move k_next into slot q
      q = q_next; // free up slot q_next
    }
  }

public:
  friend ostream &operator<<(ostream &os, const HashTableOpenAddressingNonDelete &h) {
    for (const auto &l : h.T) {
      if (l != HashTableOpenAddressingNonDelete::NIL) {
        os << l << ' ';
      }
    }
    return os;
  }
};

} // namespace frozenca

#endif //__CLRS4_OPEN_ADDRESSING_H__
