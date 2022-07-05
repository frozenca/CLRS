#ifndef __CLRS4_POINT_H__
#define __CLRS4_POINT_H__

#include <common.h>
#include <compare>
#include <cmath>

namespace frozenca {

using namespace std;

struct Point2d {
  float x_ = 0.0f;
  float y_ = 0.0f;
};

ostream& operator<<(ostream& os, const Point2d& p) {
  os << '(' << p.x_ << ',' << p.y_ << ')';
  return os;
}

float dist(const Point2d& p1, const Point2d& p2) {
  return hypot(p1.x_ - p2.x_, p1.y_ - p2.y_);
}

partial_ordering operator<=> (const Point2d& p1, const Point2d& p2) {
  if (auto cmp = p1.x_ <=> p2.x_; cmp != partial_ordering::equivalent) {
    return cmp;
  }
  return p1.y_ <=> p2.y_;
}

struct Point3d {
  float x_ = 0.0f;
  float y_ = 0.0f;
  float z_ = 0.0f;
};

ostream& operator<<(ostream& os, const Point3d& p) {
  os << '(' << p.x_ << ',' << p.y_ << ',' << p.z_ << ')';
  return os;
}

float dist(const Point3d& p1, const Point3d& p2) {
  return hypot(p1.x_ - p2.x_, p1.y_ - p2.y_, p1.z_ - p2.z_);
}

partial_ordering operator<=> (const Point3d& p1, const Point3d& p2) {
  if (auto cmp = p1.x_ <=> p2.x_; cmp != partial_ordering::equivalent) {
    return cmp;
  }
  if (auto cmp = p1.y_ <=> p2.y_; cmp != partial_ordering::equivalent) {
    return cmp;
  }
  return p1.z_ <=> p2.z_;
}

} // namespace frozenca

#endif //__CLRS4_POINT_H__
