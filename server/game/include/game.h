#pragma once

#include <string>
#include <set>
#include <vector>
#include <optional>

#include "point.h"
#include "types.h"
#include "ant.h"
#include "hill.h"
#include "food.h"
#include "data.h"
#include "player_manager.h"
#include "logger.h"
#include "observer.h"
#include "map.h"

namespace constants {
    static int MAX_FOOD_COUNT = 50;
    static int MAX_TURN_COUNT = 300;
}

class Game {
public:
	/// @brief Create game from configuration file
	/// @param game_config Path to game configuration file
	/// @param player_config Path to player configuration file
	/// @return Game initialized game object or nullopt
	static std::optional<Game> load_config(const std::string&, const std::string&);

	/// @brief Start game
	void run();

protected:
    Game(Map&& world_map, PlayerManager&& player_manager, Logger&& logger, Observer&& observer) : world_map(std::move(world_map)), player_manager(std::move(player_manager)), logger(std::move(logger)), observer(std::move(observer)) {};
	int current_turn = 0;
	AntMap alive_ants;
	PlayerMap alive_players;
	std::set<Hill, HillComparator> alive_hills;
	std::set<Food, FoodComparator> alive_food;
	std::map<PlayerID, std::set<HillID>> player_hills;

	Map world_map;
	PlayerManager player_manager;
	Logger logger;
	Observer observer;

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
