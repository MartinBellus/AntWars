#pragma once

#include <string>
#include <compare>
#include <map>
#include "types.h"

class Player {
public:
	Player(std::string name, PlayerID id): name(name), id(id) {};
	Player() {};
	std::string get_name() const { return name; }
	PlayerID get_id() const { return id; }
	int get_score() const { return score; }
	void update_score(int delta) { score += delta; }
	int get_food_count() const { return food_count; }
	void inc_food_count() { food_count++; }
	void dec_food_count() { food_count--; }
	auto operator <=> (const Player& other) const {return id <=> other.id;}
private:
	std::string name;
	int score = 0;
	int food_count = 0;
	PlayerID id;
};

typedef std::map<PlayerID, Player> PlayerMap ;

/// @brief Insert player into the map and consume it
void insert(PlayerMap&, Player&&);
