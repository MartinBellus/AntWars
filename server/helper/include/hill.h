#pragma once

#include <string>
#include <compare>
#include "types.h"
#include "point.h"

class Hill { // TODO add interface
public:
	Hill(Point position, PlayerID owner) : position(position), owner(owner), id(++id_counter) {};
	Point get_position() const {return position;}
	PlayerID get_owner() const {return owner;}
	HillID get_id() const {return id;}
	auto operator <=>(const Hill& other) const {return id <=> other.id;}
private:
	Point position;
	PlayerID owner;
	HillID id;
	static inline HillID id_counter = HillID(0);
};

struct HillComparator{
	using is_transparent = void;
	bool operator()(const Hill& a, const Hill& b) const {return a < b;}
	bool operator()(const Hill& a, const HillID& b) const {return a.get_id() < b;}
	bool operator()(const HillID& a, const Hill& b) const {return a < b.get_id();}
};
