#pragma once

#include <utility>
#include <vector>
#include <map>
#include <sstream>

#include "types.h"
#include "point.h"

struct ObserverTurn {
	std::map<AntID, Point> ant_positions;

};

struct PlayerTurn {

};

struct Moves {
	std::vector<std::pair<AntID, Point>> data;
	static std::optional<Moves> from_sstream(std::stringstream&);
};
