#pragma once

#include <utility>
#include <set>
#include <vector>
#include "types.h"
#include "ant.h"
#include "food.h"

std::vector<std::pair<FoodID,PlayerID>> get_harvested_food(const std::set<Ant, AntComparator> &, const std::set<Food,FoodComparator> &);
