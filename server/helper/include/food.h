#pragma once

#include <compare>
#include "point.h"
#include "types.h"

class Food {
public:
	Food(Point position) : position(position), id(++id_counter) {}
	Point get_position() const {return position;}
	FoodID get_id() const {return id;}
	auto operator <=>(const Food& other) const {return id <=> other.id;}
private:
	Point position;
	FoodID id;
	static inline FoodID id_counter = FoodID(0);
};

struct FoodComparator{
	using is_transparent = void;
	bool operator()(const Food& a, const Food& b) const {return a < b;}
	bool operator()(const Food& a, const FoodID& b) const {return a.get_id() < b;}
	bool operator()(const FoodID& a, const Food& b) const {return a < b.get_id();}
};
