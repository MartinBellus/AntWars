#pragma once

#include <string>
#include <set>
#include <vector>
#include <expected>

#include "point.h"
#include "types.h"
#include "ant.h"
#include "hill.h"
#include "food.h"
#include "data.h"
#include "player_manager.h"
#include "logger.h"

class Game {
public:
	/// @brief Create game from configuration file
	/// @param path Path to configuration file
	/// @return Game initialized game object
	static std::expected<Game, std::string> load_config(const std::string& path);

	/// @brief Start game
	void run();

protected:
	int player_count;
	int current_turn = 0;
	AntMap alive_ants;
	PlayerMap alive_players;
	std::set<Hill, HillComparator> alive_hills;
	std::set<Food, FoodComparator> alive_food;

	PlayerManager player_manager;
	Logger logger;

	void game_loop();
	void init();
	bool check_end();
	void cleanup();

	void kill_ant(AntID);
	void kill_hill(HillID);
	void kill_player(PlayerID);
	void harvest_food(FoodID, PlayerID);
	void move_ant(AntID, Point);

	void handle_player_moves(const Player&, const Moves&); 
};
