#include "point.h"

long long Point::dist2() const {
	return x * x + y * y;
}

Point operator+(const Point& lhs, const Point& rhs) {
	return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

Point operator-(const Point& lhs, const Point& rhs) {
	return Point(lhs.x - rhs.x, lhs.y - rhs.y);
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
	os << "(x = " << p.x << ", y = " << p.y << ")";
	return os;
}

std::vector<Point> points_in_circle(Point center, int r2){
	std::vector<Point> points;
	int mx = 0;
	for(;mx*mx < r2;mx++);

	for(int dx = -mx; dx <= mx;dx++){
		for(int dy = -mx; dy <= mx;dy++){
			if(dx*dx + dy*dy <= r2){
				points.push_back(Point(center.x + dx, center.y + dy));
			}
		}
	}

	return points;
}
