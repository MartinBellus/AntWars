#pragma once

#include <sstream>
#include <string>
#include <map>

#include "data.h"

namespace format {
	std::string to_observer(const ObserverTurn&);
	std::string init_observer(const ObserverInit&);
	std::string to_player(const PlayerTurn&);
	std::string init_player(const PlayerInit&);
	std::string observer_scores(const std::map<std::string, int>&);
}
