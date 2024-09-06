#include "game.h"
#include "attack_phase.h"
#include "gather_phase.h"
#include "razing_phase.h"

using namespace std;

void Game::run() {
	init();
	do {
		game_loop();
	} while(!check_end());
	cleanup();
}

void Game::init() {
	// sent data to observer
	//
	// run player processes
	//
	// send data to players
}

void Game::game_loop() {
	logger.log("STARTING TURN: " + to_string(current_turn));
	// handle player moves
	//
	for(auto &[_, player] : alive_players) {
		// TODO: prepare payload and send turn to player
		PlayerID player_id = player.get_id();
		Status status;
		stringstream ss = player_manager.read_player(player_id, status);

		if(status == Status::Tle) {
			player_manager.log_player(player_id, "Killing player: Time limit exceeded");
			kill_player(player_id);
		}else if(status == Status::Err) {
			player_manager.log_player(player_id, "Killing player: Bot has crashed");
			kill_player(player_id);
		}

		auto player_moves = Moves::from_sstream(ss);
		if(!player_moves.has_value()) {
			logger.log("Could not parse data from player " + player.get_name());
			continue;
		}
		handle_player_moves(player, player_moves.value());
	}
	//  check for collisions
	//
	// attack phase
	//
	for(AntID killed_ant : mark_killed_ants(alive_ants)){
		kill_ant(killed_ant);
	}

	// razing phase
	//
	for(HillID razed_hill : mark_razed_hills(alive_ants, alive_hills)){
		kill_hill(razed_hill);
	}
	// spawn ants
	//
	// gather phase
	//
	for(auto [food, owner] : get_harvested_food(alive_ants, alive_food)){
		harvest_food(food, owner);
	}
	// spawn food
	//
	current_turn++;
}

void Game::cleanup() {
	// save scores
	//
	// clean up all players
	//
}

bool Game::check_end() {
	// TODO: add more end conditions
	if(alive_players.size() == 1) {
		return true;
	}
	return false;
}

void Game::kill_player(PlayerID player_id) {
	Player player = alive_players.find(player_id)->second;
	alive_players.erase(player_id);
	player_manager.kill_player(player_id);
	logger.log("Player " + player.get_name() + " has been killed");
}

void Game::kill_ant(AntID ant_id) {
	alive_ants.erase(ant_id);
}

void Game::kill_hill(HillID hill_id) {
	Hill hill = *alive_hills.find(hill_id);
	alive_hills.erase(hill);
}

void Game::harvest_food(FoodID food_id, PlayerID player_id) {
	Food food = *alive_food.find(food_id);
	alive_food.erase(food);
	if(player_id != PlayerID::NONE) {
		Player &p = alive_players[player_id];
		p.inc_food_count();
	}
}
