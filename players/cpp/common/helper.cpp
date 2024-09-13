#include "data.h"

Point Point::operator+(const Point &p) const {
    return Point(x + p.x, y + p.y);
}

Point Point::operator-(const Point &p) const {
    return Point(x - p.x, y - p.y);
}

long long Point::dist2() const {
    return (long long) x * x + (long long) y * y;
}

bool Map::is_inside(const Point& p) const {
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}

bool Map::can_move_to(const Point& p) const {
    return is_inside(p) && water_tiles.find(p) == water_tiles.end();
}
