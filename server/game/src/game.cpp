#include "game.h"
#include "attack_phase.h"

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
	for(const AntID &killed_ant : mark_killed_ants(alive_ants)){
		kill_ant(killed_ant);
	}

	// razing phase
	//
	// spawn ants
	//
	// gather phase
	//
	// spawn food
	//
}

void Game::cleanup() {
	// save scores
	//
	// clean up all players
	//
}
