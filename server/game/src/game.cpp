#include "game.h"
#include "attack_phase.h"
#include "gather_phase.h"
#include "razing_phase.h"
#include "data.h"

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
	// logger.log("STARTING TURN: " + to_string(current_turn));
	// handle player moves
	//
	for(Player player : alive_players) {
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

		Moves player_moves = Moves::from_sstream(ss);
		for(auto [ant, new_position] : player_moves.data) {
			if(alive_ants.find(ant) == alive_ants.end()) {
				// TODO: Log ant does not exist

				continue;
			}
			// TODO: move ant
		}
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
	Player player = *alive_players.find(player_id);
	alive_players.erase(player);
	player_manager.kill_player(player_id);
	// logger.log("Player " + player.get_name() + " has been killed");
}

void Game::kill_ant(AntID ant_id) {
	Ant ant = *alive_ants.find(ant_id);
	alive_ants.erase(ant);
}

void Game::kill_hill(HillID hill_id) {
	Hill hill = *alive_hills.find(hill_id);
	alive_hills.erase(hill);
}

void Game::harvest_food(FoodID food_id, PlayerID player_id) {
	Food food = *alive_food.find(food_id);
	alive_food.erase(food);
	if(player_id != PlayerID::NONE) {
		Player player = *alive_players.find(player_id);
		player.inc_food_count();
	}
}
