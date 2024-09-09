#pragma once

#include "point.h"
#include <set>
#include <optional>
#include <string>

class Map {
public:
    Map(int width, int height) : width(width), height(height) {}
    static std::optional<Map> from_ppm(const std::string&);
    bool inside(const Point&) const;
    bool can_move_to(const Point&) const;
    const std::set<Point>& get_water_tiles() const { return water_tiles; };
    const std::set<Point>& get_hill_positions() const { return hill_positions; }
private:
    int width, height;
    std::set<Point> water_tiles;
    std::set<Point> hill_positions;
};
