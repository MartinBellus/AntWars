#pragma once

#include <string>
#include <set>
#include <vector>
#include <expected>

#include "types.h"
#include "ant.h"
#include "hill.h"
#include "food.h"

class Game {
public:
	/// @brief Create game from configuration file
	/// @param path Path to configuration file
	/// @return Game initialized game object
	friend std::expected<Game, unsigned> load_config(const std::string& path);

	/// @brief Start game
	void run();

protected:
	int player_count;
	std::set<Ant, AntComparator> alive_ants;
	std::set<Hill, HillComparator> alive_hills;
	std::set<Food, FoodComparator> alive_food;

	void game_loop();
	void init();
	bool check_end();
	void cleanup();

	void kill_ant(AntID);
	void kill_hill(HillID);
	void harvest_food(FoodID, PlayerID);
};
