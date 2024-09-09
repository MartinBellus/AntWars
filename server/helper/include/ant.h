#pragma once

#include <compare>
#include <map>
#include "types.h"
#include "point.h"

namespace constants{
	inline int ANT_BATTLE_RADIUS2 = 5;
	inline int ANT_GATHER_RADIUS2 = 1;
	inline int ANT_MOVE_RADIUS2 = 1;
}

class Ant {
public:
	Ant(Point position, PlayerID owner) : position(position), owner(owner), id(++id_counter) {}
	Ant() {}
	Point get_position() const {return position;}
	void set_position(Point p) {position = p;}
	PlayerID get_owner() const {return owner;}
	AntID get_id() const {return id;}
	auto operator <=>(const Ant& other) const {return id <=> other.id;}
private:
	Point position;
	PlayerID owner;
	AntID id;
	static inline AntID id_counter = AntID(0);
};

typedef std::map<AntID, Ant> AntMap ;

void insert(AntMap&, Ant);
