#include "game.h"
#include "attack_phase.h"
#include "gather_phase.h"
#include "razing_phase.h"

using namespace std;

void Game::run() {
	init();
	do {
		game_loop();
	} while(check_end());
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
	// get moves from players
	//
	// move ants and check for collisions
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
}

void Game::cleanup() {
	// save scores
	//
	// clean up all players
	//
}
