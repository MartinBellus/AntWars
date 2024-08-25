#pragma once

#include <string>
#include <compare>
#include "types.h"

class Player {
public:
	Player(std::string name, PlayerID id): name(name), id(id) {};
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

struct PlayerComparator{
	using is_transparent = void;
	bool operator()(const Player& a, const Player& b) const {return a < b;}
	bool operator()(const Player& a, const PlayerID& b) const {return a.get_id() < b;}
	bool operator()(const PlayerID& a, const Player& b) const {return a < b.get_id();}
};
