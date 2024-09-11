#pragma once

#include <vector>
#include <set>
#include "ant.h"
#include "hill.h"
#include "point.h"
#include "types.h"

std::vector<Point> get_new_ant_pos(const std::set<HillID>&, const std::set<Hill, HillComparator>&, const AntMap&);
