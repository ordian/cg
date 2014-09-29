#include "../common/common.hpp"

enum SegmentRayIntersection {
  YES,
  NO,
  BOUNDARY
};

bool withinStrip(Int l, Int h, Point const &r) {
  return l <= r.y && r.y <= h;
}

SegmentRayIntersection
segmentHRayIntersection(Point a, Point b, Point const &r) {
  if (b < a) {
    std::swap(a, b);
  }
  if (!(withinStrip(a.y, b.y, r))) {
    return NO;
  } else if ((a.y >= r.y) != (b.y >= r.y)) {
    switch (orientation(a, b, r)) {
      case LEFT:      return YES;
      case RIGHT:     return NO;
      case COLLINEAR: return BOUNDARY;
    }
  } else if (a.y == r.y && b.y == r.y) {
    return
      r.x < std::min(a.x, b.x) ? YES :
      r.x > std::max(a.x, b.x) ?  NO : BOUNDARY;
  }
  return NO;
}

bool contains(Polygon const &poly, Point const &p) {
  if (!poly.getBounds().contains(p))
    return false;

  bool c = false, skip = false;
  std::vector<Point> const &points = poly.getPoints();
  for (size_t i = 0, size = points.size(), j = size - 1; i != size; ) {
    Point const &a = points[j], &b = points[i];
    switch (segmentHRayIntersection(skip ? points[i - 1] : a, b, p)) {
      case NO:
        j = i++;
        continue;
      case BOUNDARY:
        return true;
      case YES:
        if (b.y != p.y) {
          if ((a.y >= p.y) != (b.y >= p.y)) {
            c = !c;
          }
          j = i++;
          skip = false;
        } else {
          ++i;
          skip = true;
        }
    }
  }
  return c;
}

int main() {
  std::ios::sync_with_stdio(false);
  Polygon poly;
  std::cin >> poly;

  size_t m = 0;
  std::cin >> m;
  Point p;
  for (size_t i = 0; i != m; ++i) {
    std::cin >> p;
    std::cout << (contains(poly, p) ? "yes" : "no") << std::endl;
  }

  return 0;
}
