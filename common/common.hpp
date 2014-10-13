#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>

typedef std::int32_t Int;
typedef std::int64_t Long;

std::istream &skip(std::istream &input, char until) {
  return input.ignore(std::numeric_limits<std::streamsize>::max(), until);
}

struct Point {
  Int x, y;

  Point() : x(0), y(0) {}
  Point(Int a, Int b) : x(a), y(b) {}

  friend std::istream &operator>>(std::istream &input, Point &p) {
    skip(input, '(') >> p.x;
    skip(input, ',') >> p.y;
    return skip(input, ')');
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

struct Polygon {
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

std::istream &operator>>(std::istream &input, Polygon &poly) {
  size_t n = 0;
  input >> n;
  poly.points.clear();
  poly.bounds.clear();
  poly.points.resize(n);

  for (size_t i = 0; i != n; ++i) {
    input >> poly.points[i];
    poly.bounds.update(poly.points[i]);
  }

  return input;
}

enum Orientation {
  LEFT,
  COLLINEAR,
  RIGHT
};

Long det(Long a, Long b, Long c, Long d) {
  return a * d - b * c;
}

Orientation orientation(Point const &p, Point const &q, Point const &r) {
  Long x = det(
    (q.x - p.x), (r.x - p.x),
    (q.y - p.y), (r.y - p.y));
  return x < 0 ? LEFT : x > 0 ? RIGHT : COLLINEAR;
}

