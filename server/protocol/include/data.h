#pragma once

#include <utility>
#include <vector>
#include <map>
#include <set>
#include <sstream>

#include "food.h"
#include "types.h"
#include "point.h"
#include "hill.h"
#include "ant.h"
#include "player.h"
#include "map.h"

struct ObserverTurn {
    const AntMap& alive_ants;
    const std::set<Hill, HillComparator>& alive_hills;
    const std::set<Food, FoodComparator>& alive_food;
    const PlayerMap& alive_players;
    int turn_num;
};

struct ObserverInit {
    const PlayerMap& players;
    const Map& map;
    const std::set<Hill, HillComparator>& spawn_positions;
};

struct PlayerTurn {
    const AntMap& alive_ants;
    const std::set<Hill, HillComparator>& alive_hills;
    const std::set<Food, FoodComparator>& alive_food;
    int food_count;
    int turn_num;
};

struct PlayerInit {
    const Map map;
    const std::set<Hill, HillComparator>& spawn_positions;
    PlayerID my_id;
};

struct Moves {
	std::vector<std::pair<AntID, Point>> data;
	static std::optional<Moves> from_sstream(std::stringstream&);
};
