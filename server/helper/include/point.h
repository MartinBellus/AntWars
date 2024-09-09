#pragma once

#include <iostream>
#include <vector>

struct Point{
	int x, y;
    Point(int x,int y) : x(x), y(y){}
    Point(){}
    long long dist2() const;
    friend Point operator+(const Point &, const Point&);
    friend Point operator-(const Point &, const Point&);
    auto operator<=>(const Point&) const = default;
    friend std::ostream& operator<<(std::ostream &, const Point&);
};

// @brief: Calculate all points, that are inside the circle with center in 'center' and radius2 'r'.
// @param: center - center of the circle
// @param: r - radius of the circle squared
// @return: vector of points that are inside the circle
std::vector<Point> points_in_circle(Point, int);
