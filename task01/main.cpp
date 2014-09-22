#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

typedef std::int32_t Int;
typedef std::int64_t Long;

struct Point {
  Int x, y;

  Point() : x(0), y(0) {}
  Point(Int a, Int b) : x(a), y(b) {}

  friend std::istream &operator>>(std::istream &input, Point &p) {
    input.ignore(1, '(');
    input >> p.x;
    input.ignore(1, ',');
    input >> p.y;
    input.ignore(1, ')');
    return input;
  }

  bool operator<(Point &p) {
    return (y < p.y) || ((y == p.y) && (x < p.x));
  }
};

struct Box {
  Int minX, maxX;
  Int minY, maxY;

  void update(Point const &p) {
    if (empty) {
      minX = maxX = p.x;
      minY = maxY = p.y;
      empty = false;
    } else {
      minX = std::min(minX, p.x);
      minY = std::min(minY, p.y);
      maxX = std::max(maxX, p.x);
      maxY = std::max(maxY, p.y);
    }
  }

  bool contains(Point const &p) const {
    return p.x >= minX && p.y >= minY && p.x <= maxX && p.y <= maxY;
  }

  void clear() {
    empty = true;
  }

private:
  bool empty = true;
};

enum Orientation {
  LEFT,
  COLLINEAR,
  RIGHT
};

Long det(Long a, Long b, Long c, Long d) {
  return a * c - b * d;
}

Orientation orientation(Point const &u, Point const &v, Point const &w) {
  Long x = det(
    (Long) v.x - u.x, (Long) w.x - u.x,
    (Long) w.y - u.y, (Long) v.y - u.y);
  return x < 0 ? RIGHT : x > 0 ? LEFT : COLLINEAR;
}

struct Polygon {
  bool contains(Point const &point) const;

  std::vector<Point> const &getPoints() const {
    return points;
  }

  Box const &getBounds() const {
    return bounds;
  }

  friend std::istream &operator>>(std::istream &input, Polygon &poly);

private:
  std::vector<Point> points;
  Box bounds;
};

enum SegmentRayIntersection {
  YES,
  NO,
  EDGE
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
      case COLLINEAR: return EDGE;
    }
  } else if (a.y == r.y && b.y == r.y) {
    return
      r.x < std::min(a.x, b.x) ? YES :
      r.x > std::max(a.x, b.x) ?  NO : EDGE;
  }
  return NO;
}

bool Polygon::contains(Point const &p) const {
  if (!bounds.contains(p))
    return false;

  bool c = false;
  for (size_t i = 0, size = points.size(), j = size - 1; i != size; ) {
    Point const &a = points[j], &b = points[i];
    switch (segmentHRayIntersection(a, b, p)) {
      case NO:
        j = i++;
        continue;
      case EDGE:
        return true;
      case YES:
        if (b.y != p.y) {
          if ((a.y >= p.y) != (b.y >= p.y)) {
            c = !c;
          }
          j = i++;
        } else {
          ++i;
        }
    }
  }
  return c;
}


std::istream &operator>>(std::istream &input, Polygon &poly) {
  size_t n = 0;
  input >> n;
  poly.points.clear();
  poly.bounds.clear();
  poly.points.reserve(n);

  Point p;
  for (size_t i = 0; i != n; ++i) {
    input >> p;
    poly.bounds.update(p);
    poly.points.push_back(p);
  }

  auto lowest = std::min_element(poly.points.begin(), poly.points.end());
  std::rotate(poly.points.begin(), lowest, poly.points.end());

  return input;
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
    std::cout << (poly.contains(p) ? "yes" : "no") << std::endl;
  }

  return 0;
}
