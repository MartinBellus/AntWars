#pragma once

#include <map>
#include <set>

struct Point {
    int x, y;
    Point operator+(const Point&) const;
    Point operator-(const Point&) const;
    auto operator<=>(const Point&) const = default;
    long long dist2() const;
};

typedef int AntID;
typedef int PlayerID;
typedef int HillID;

struct Move {
    AntID ant;
    Point to;
};

struct Map {
    int width, height;
    std::set<Point> water_tiles;
    bool is_inside(const Point&) const;
    bool can_move_to(const Point&) const;
};

struct Ant {
    Point pos;
    PlayerID owner;
    AntID id;
};

struct Hill {
    Point pos;
    PlayerID owner;
    HillID id;
};

struct Food {
    Point pos;
    auto operator<=>(const Food&) const = default;
};

///@brief Struct representing current game state
struct World {
    int my_id;
    int food_count;
    int turn = 0;
    Map map;
    std::map<AntID, Ant> alive_ants;
    std::set<HillID> alive_hills;
    std::map<HillID, Hill> hills;
    std::set<Food> food;
};
