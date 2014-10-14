#include "../common/common.hpp"
#include <functional>

namespace /* details */ {
typedef std::function<bool(Point const &a,
                           Point const &b,
                           Point const &c,
			   bool isLeft)> Classifier;

Classifier Left = [](Point const &a,
		     Point const &b,
		     Point const &c,
		     bool isLeft) {
  switch (classify(a, b, c)) {
    case PointSegmentPosition::LEFT: return true;
    case PointSegmentPosition::BETWEEN: return isLeft;
    case PointSegmentPosition::BEYOND: return !isLeft;
    default: return false;
  }
};

Classifier Right = [](Point const &a,
		      Point const &b,
		      Point const &c,
		      bool isLeft) {
  switch (classify(a, b, c)) {
    case PointSegmentPosition::RIGHT: return true;
    case PointSegmentPosition::BETWEEN: return !isLeft;
    case PointSegmentPosition::BEYOND: return isLeft;
    default: return false;
  }
};

int tangent(Polygon const &poly, 
            Point const &p,
            bool isLeft = false, 
            Classifier left =  Left, 
            Classifier right = Right) {
  if (isLeft)
    std::swap(left, right);
  auto v = poly.getPoints();
  int n = v.size();
  int l = 0, r = n, m = 0;
  if (right(p, v[isLeft ? n-1 : 1], v[0], isLeft) && 
      !left(p, v[isLeft ? 1 : n-1], v[0], isLeft))
    return 0;

  while (l + 1 != r) {
    m = (l + r) / 2;
    bool d = isLeft 
      ? !left(p, v[m+1], v[m], isLeft) 
      : right(p, v[m+1], v[m], isLeft);
    if (d && (isLeft 
	      ? right(p, v[m-1], v[m], isLeft) 
	      : !left(p, v[m-1], v[m], isLeft)))
      return m;
    
    if (left(p, v[l+1], v[l], isLeft)) 
      ((d || left(p, v[l], v[m], isLeft)) ? r : l) = m;
    else
      ((!d || !right(p, v[l], v[m], isLeft)) ? l : r) = m;
  }
  return l;
}
}

std::pair<int, int> tangentPoints(Polygon const &poly, Point const &p) {
  int r = tangent(poly, p);
  int l = tangent(poly, p, true);
  return std::make_pair(r, l);
}

int main() {
  std::ios::sync_with_stdio(false);
  Polygon poly;
  Point p;
  std::size_t m;
  int l, r;
  std::cin >> poly;
  
  std::cin >> m;
  for (std::size_t i = 0; i != m; ++i) {
    std::cin >> p;
    std::tie(l, r) = tangentPoints(poly, p); 
    std::cout << l << " " << r << "\n";
  }
  return 0;
}
