#pragma once

#include <string>
#include <compare>
#include "types.h"
#include "point.h"

namespace constants{
	inline int ANT_BATTLE_RADIUS2 = 5;
	inline int ANT_GATHER_RADIUS2 = 1;
}

class Ant { // TODO add interface
public:
	Ant(Point position, PlayerID owner) : position(position), owner(owner), id(++id_counter) {}
	Point get_position() const {return position;}
	PlayerID get_owner() const {return owner;}
	AntID get_id() const {return id;}
	auto operator <=>(const Ant& other) const {return id <=> other.id;}
private:
	Point position;
	PlayerID owner;
	AntID id;
	static inline AntID id_counter = AntID(0);
};

struct AntComparator{
	using is_transparent = void;
	bool operator()(const Ant& a, const Ant& b) const {return a < b;}
	bool operator()(const Ant& a, const AntID& b) const {return a.get_id() < b;}
	bool operator()(const AntID& a, const Ant& b) const {return a < b.get_id();}
};
