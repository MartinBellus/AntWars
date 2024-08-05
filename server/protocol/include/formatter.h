#pragma once

#include <sstream>
#include <string>

#include "data.h"

class Formatter{
public:
	Formatter();
	std::string to_observer(const ObserverTurn&);
	std::string to_player(const PlayerTurn&);
};
