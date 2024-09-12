#include "game.h"
#include "ant_spawn_phase.h"
#include "attack_phase.h"
#include "check_collisions.h"
#include "food_spawn_phase.h"
#include "format.h"
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
    ObserverInit observer_init{alive_players, world_map, alive_hills};
    observer.send(format::init_observer(observer_init));

    // run player processes
    //
    // send data to players
    PlayerInit player_init{world_map, alive_hills, PlayerID::NONE};
    for(const auto& [id, player] : alive_players) {
        player_init.my_id = id;
        player_manager.send_player(id, format::init_player(player_init));
    }
    // check if all players are ready
    vector<PlayerID> not_responding;
    for(const auto& [id, player] : alive_players) {
        Status status;
        stringstream ss = player_manager.read_player(id, status);
        if(status != Status::OK) {
            logger.log("Player " + player.get_name() + " failed to initialize");
            not_responding.push_back(id);
        }
    }
    for(PlayerID id : not_responding) {
        kill_player(id);
    }
}

void Game::game_loop() {
    logger.log("STARTING TURN: " + to_string(current_turn));
    // handle player moves
    //
    PlayerTurn this_turn{alive_ants, alive_hills, alive_food, 0, current_turn};
    // send data to players
    for(auto &[_, player] : alive_players) {
       PlayerID player_id = player.get_id();
       this_turn.food_count = player.get_food_count();
       player_manager.send_player(player_id, format::to_player(this_turn));
    }
    vector<PlayerID> not_responding;
    for(auto &[_, player] : alive_players) {
        PlayerID player_id = player.get_id();
        Status status;
        stringstream ss = player_manager.read_player(player_id, status);

        if(status == Status::TLE) {
            logger.log("Killing player " + player.get_name() + ": Time limit exceeded");
            not_responding.push_back(player_id);
            continue;
        }else if(status == Status::ERR) {
            logger.log("Killing player " + player.get_name() + ": Player has crashed");
            not_responding.push_back(player_id);
            continue;
        }else if(status == Status::END) {
            logger.log("Killing player " + player.get_name() + ": Player terminated too soon");
            not_responding.push_back(player_id);
            continue;
        }

        auto player_moves = Moves::from_sstream(ss);
        if(!player_moves.has_value()) {
            logger.log("Could not parse data from player " + player.get_name());
            continue;
        }
        handle_player_moves(player, player_moves.value());
    }

    for(PlayerID id : not_responding) {
        kill_player(id);
    }

    //  check for collisions
    //
    for(AntID ant_id : get_colliding_ants(alive_ants)) {
        kill_ant(ant_id);
    }
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
    for(auto &[player_id, player] : alive_players) {
        for(Point p : get_new_ant_pos(player_hills[player_id], alive_hills, alive_ants)) {
            if(player.get_food_count()) {
                insert(alive_ants, Ant(p, player_id));
                player.dec_food_count();
            }
        }
    }
    // gather phase
    //
    for(auto [food, owner] : get_harvested_food(alive_ants, alive_food)){
        harvest_food(food, owner);
    }
    // spawn food
    //
    for(Point p : get_new_food_locations((constants::MAX_FOOD_COUNT - alive_food.size())/2, world_map)) {
        alive_food.insert(Food(p));
    }
    // send data to observer
    //
    ObserverTurn observer_turn{alive_ants, alive_hills, alive_food, alive_players, current_turn};
    observer.send(format::to_observer(observer_turn));

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
    if(alive_players.size() == 0) {
        logger.log("Game end: No players left");
        return true;
    }
    if(alive_players.size() == 1) {
        logger.log("Game end: Last player alive");
        return true;
    }
    if(current_turn >= constants::MAX_TURN_COUNT) {
        logger.log("Game end: Max turn count reached");
        return true;
    }
    return false;
}

void Game::kill_player(PlayerID player_id) {
    Player& player = alive_players[player_id];
    player_manager.kill_player(player_id);
    logger.log("Player " + player.get_name() + " has been killed");
    alive_players.erase(player_id);
}

void Game::kill_ant(AntID ant_id) {
    alive_ants.erase(ant_id);
}

void Game::kill_hill(HillID hill_id) {
    Hill hill = *alive_hills.find(hill_id);
    PlayerID owner = hill.get_owner();
    player_hills[owner].erase(hill_id);
    alive_hills.erase(hill);
    if(player_hills[owner].size() == 0) {
        kill_player(owner);
    }
}

void Game::harvest_food(FoodID food_id, PlayerID player_id) {
    Food food = *alive_food.find(food_id);
    alive_food.erase(food);
    if(player_id != PlayerID::NONE) {
        Player &p = alive_players[player_id];
        p.inc_food_count();
    }
}
