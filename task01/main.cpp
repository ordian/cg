#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

template<typename T>
struct point {
  T x, y;

  point() : x(0), y(0) {}
  point(T x_, T y_) : x(x_), y(y_) {}

  friend std::istream &operator>>(std::istream &input, point<T> &p) {
    input.ignore(1, '(');
    input >> p.x;
    input.ignore(1, ',');
    input >> p.y;
    input.ignore(1, ')');
    return input;
  }

  bool operator<(point<T> &p) {
    return y < p.y || y == p.y && x < p.x;
  }
};

template<typename T>
struct box {
  T minX, maxX;
  T minY, maxY;

  void update(point<T> const &p) {
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

  bool contains(point<T> const &p) const {
    return p.x >= minX && p.y >= minY && p.x <= maxX && p.y <= maxY;
  }

  void clear() {
    empty = true;
  }

private:
  bool empty = true;
};

template<typename T>
struct polygon {
  bool contains(point<T> const &point) const;

  template<typename S>
  friend std::istream &operator>>(std::istream &input, polygon<S> &poly);

  std::vector<point<T>> const &getPoints() const {
    return points;
  }

  box<T> const &getBounds() const {
    return bounds;
  }

private:
  std::vector<point<T>> points;
  box<T> bounds;
};

template<typename T>
bool magic(point<T> const &i, point<T> const &j, point<T> const &p) {
  return (i.y > p.y) != (j.y > p.y) &&
    (j.y - i.y) * (p.x - i.x) < (j.x - i.x) * (p.y - i.y);
}

template<typename T>
bool polygon<T>::contains(point<T> const &p) const {
  if (!bounds.contains(p))
    return false;

  size_t size = points.size();
  bool c = true;
  for (size_t i = 0, j = size - 1; i != size; j = i++) {
    if (magic(points[i], points[j], p))
      c = !c;
  }
  return c;
}


template<typename T>
std::istream &operator>>(std::istream &input, polygon<T> &poly) {
  size_t n = 0;
  std::cin >> n;
  poly.points.clear();
  poly.bounds.clear();
  poly.points.reserve(n);

  point<T> p;
  for (size_t i = 0; i != n; ++i) {
    std::cin >> p;
    poly.bounds.update(p);
    poly.points.push_back(p);
  }

  auto lowest = std::min_element(poly.points.begin(), poly.points.end());
  std::rotate(poly.points.begin(), lowest, poly.points.end());

  return input;
}


int main() {
  std::ios::sync_with_stdio(false);
  polygon<int> poly;
  std::cin >> poly;

  size_t m = 0;
  std::cin >> m;
  point<int> p;
  for (size_t i = 0; i != m; ++i) {
    std::cin >> p;
    std::cout << (poly.contains(p) ? "yes" : "no") << std::endl;
  }

  return 0;
}
