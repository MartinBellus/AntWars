#pragma once

#include <string>
#include <compare>
#include "types.h"

class Player {
public:
	Player(std::string name): name(name), id(++id_counter) {};
	std::string get_name() const { return name; }
	int get_score() const { return score; }
	void update_score(int delta) { score += delta; }
	auto operator <=> (const Player& other) const {return id <=> other.id;}
private:
	std::string name;
	int score = 0;
	PlayerID id;
	static inline PlayerID id_counter = PlayerID(0);
};
