#pragma once

enum class PlayerID : unsigned short {
	NONE = 0,
};
inline PlayerID operator++(PlayerID& id) { return id = PlayerID(static_cast<unsigned short>(id) + 1); }

enum class AntID : unsigned short {};
inline AntID operator++(AntID& id) { return id = AntID(static_cast<unsigned short>(id) + 1); }

enum class HillID : unsigned short {};
inline HillID operator++(HillID& id) { return id = HillID(static_cast<unsigned short>(id) + 1); }

enum class FoodID : unsigned short {};
inline FoodID operator++(FoodID& id) { return id = FoodID(static_cast<unsigned short>(id) + 1); }
