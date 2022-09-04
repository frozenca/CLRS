#ifndef __CLRS4_GRAPH_PROPERTIES_H__
#define __CLRS4_GRAPH_PROPERTIES_H__

#include <common.h>
#include <concepts>
#include <functional>
#include <hashfunction.h>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace frozenca {

using namespace std;

template <Descriptor V> struct Hash<Edge<V>> {
  size_t operator()(const Edge<V> &e) const {
    return Hash<V>{}(e.second) + 1234567ULL * Hash<V>{}(e.first);
  }
};

template <Descriptor V> struct Hash<UndirectedEdge<V>> {
  size_t operator()(const UndirectedEdge<V> &e) const {
    auto f = e.first;
    auto s = e.second;
    if (f > s) {
      swap(f, s);
    }
    return Hash<V>{}(s) + 1234567ULL * Hash<V>{}(f);
  }
};

enum class GraphPropertyTag : int32_t {
  VertexBackSet,
  VertexChildren,
  VertexColor,
  VertexComponent,
  VertexDepth,
  VertexDistance,
  VertexLink,
  VertexIndegree,
  VertexOutdegree,
  VertexParent,
  VertexRank,
  VertexScc,
  VertexSize,
  VertexTime,
  VertexTimeLow,
  VertexTimeHigh,
  VertexTimeFinish,
  VertexValue,
  VertexVisited,
  VertexWeight,
  EdgeBcc,
  EdgeOrigin,
  EdgeOrientation,
  EdgeWeight,
  GraphBCCArticulationPoints,
  GraphBCCBridges,
  GraphBipartite,
  GraphEulerTour,
  GraphMBST,
  GraphMST,
  GraphPath,
  GraphSecondMST,
  GraphSpanningForest,
  GraphTopSort,
  GraphVisitSort,
};

template <> struct Hash<GraphPropertyTag> {
  size_t operator()(const GraphPropertyTag &tag) const {
    return Hash<uint32_t>{}(bit_cast<uint32_t>(tag));
  }
};

struct Property {
  virtual ~Property() {}
};

enum class VisitMark { Unvisited, Visiting, Visited };

enum class EdgeOrientation { Unoriented, Less, Greater };

template <Descriptor VertexType, typename PropertyType>
struct VertexProperty final : public Property {

  static constexpr bool int_vertex_ = is_integral_v<VertexType>;

  using ContainerType = conditional_t<int_vertex_, vector<PropertyType>,
                                      unordered_map<VertexType, PropertyType>>;

  PropertyType &operator[](const VertexType &vertex) {
    if constexpr (int_vertex_) {
      if (vertex >= ssize(vertex_properties_)) {
        vertex_properties_.resize(vertex + 1);
      }
    }
    return vertex_properties_[vertex];
  }
  const PropertyType &operator[](const VertexType &vertex) const {
    return vertex_properties_.at(vertex);
  }

  ContainerType &get() { return vertex_properties_; }

  const ContainerType &get() const { return vertex_properties_; }

private:
  ContainerType vertex_properties_;
};

template <Descriptor VertexType, bool Directed, typename PropertyType>
struct EdgeProperty final : public Property {
  using EdgeType =
      conditional_t<Directed, Edge<VertexType>, UndirectedEdge<VertexType>>;
  using ContainerType = unordered_map<EdgeType, PropertyType, Hash<EdgeType>>;
  PropertyType &operator[](const EdgeType &edge) {
    if (!Directed) {
      EdgeType edge_ = edge;
      if (edge_.first > edge_.second) {
        swap(edge_.first, edge_.second);
      }
      return edge_properties_[edge_];
    } else {
      return edge_properties_[edge];
    }
  }
  const PropertyType &operator[](const EdgeType &edge) const {
    if (!Directed) {
      EdgeType edge_ = edge;
      if (edge_.first > edge_.second) {
        swap(edge_.first, edge_.second);
      }
      return edge_properties_.at(edge_);
    } else {
      return edge_properties_.at(edge);
    }
  }

  ContainerType &get() { return edge_properties_; }

  const ContainerType &get() const { return edge_properties_; }

private:
  ContainerType edge_properties_;
};

template <Descriptor VertexType, typename PropertyType>
using DirEdgeProperty = EdgeProperty<VertexType, true, PropertyType>;

template <Descriptor VertexType, typename PropertyType>
using UndirEdgeProperty = EdgeProperty<VertexType, false, PropertyType>;

template <GraphConcept G> using EdgeSet = unordered_set<E<G>, Hash<E<G>>>;

template <typename PropertyType> struct GraphProperty final : public Property {
  PropertyType &get() { return graph_property_; }

  const PropertyType &get() const { return graph_property_; }

private:
  PropertyType graph_property_;
};

} // namespace frozenca

#endif //__CLRS4_GRAPH_PROPERTIES_H__
