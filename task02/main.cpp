#include "../common/common.hpp"
#include <deque>
#include <iterator>

struct Triangle {
  Int a, b, c;
  Triangle(Int x, Int y, Int z): a(x), b(y), c(z) {}
};

Triangle buildTriangle(Int x, Int y, Int z) {
  Int s[3] = { x, y, z };
  std::sort(s, s + 3);
  return Triangle(s[0], s[1], s[2]); 
}

std::vector<Triangle>
triangulateMonotonePolygon(Polygon const &poly) {

  std::vector<Point> const &points = poly.getPoints();
  std::vector<Triangle> result;
  std::size_t N = points.size();

  std::vector<Int> events(N);
  auto split = std::max_element(points.cbegin(), points.cend(),
      [](Point const &a, Point const &b) { return a.x < b.x; });
  auto splitIndex = std::distance(points.cbegin(), split);
  std::iota(events.begin(), events.end(), 0);
  std::reverse(events.begin() + splitIndex, events.end());
  std::inplace_merge(
      events.begin(), events.begin() + splitIndex, events.end(), 
      [&points](Int a, Int b) { return points[a].x < points[b].x; });
  auto isUpper = [splitIndex](Int a) { return a > splitIndex; };

  std::deque<Int> stack;
  stack.push_back(events[0]);
  stack.push_back(events[1]);
  
  for (size_t i = 2; i != N; ++i) {
    Int current = events[i];
    Int topStack = stack.back();
    
    if (isUpper(current) != isUpper(topStack)) {
      while (stack.size() > 1) {
        Int a = stack.back();
        stack.pop_back();
        Int b = stack.back();
        result.push_back(buildTriangle(current, a, b));
      }
      stack.pop_back();
      stack.push_back(topStack);
      stack.push_back(current);
    } else {
      while (stack.size() > 1) {
        Int a = stack.back();
        Int b = stack[stack.size() - 2];
        auto side = orientation(points[current], points[b], points[a]);
        if (side == COLLINEAR || (side == RIGHT) != isUpper(a)) 
          break;
        stack.pop_back();
        result.push_back(buildTriangle(current, a, b));
      }
      stack.push_back(current);
    }
  }
  
  return result;
}

std::ostream &operator <<(std::ostream &output, Triangle const &triangle) {
  output << "(" 
         << triangle.a << ", "   
         << triangle.b << ", "
         << triangle.c 
         << ")";
  return output;
}

int main() {
  std::ios::sync_with_stdio(false);
  Polygon poly;
  std::cin >> poly;

  auto triangles = triangulateMonotonePolygon(poly);
  std::copy(triangles.begin(), triangles.end(), 
            std::ostream_iterator<Triangle>(std::cout, "\n"));
  return 0;
}
