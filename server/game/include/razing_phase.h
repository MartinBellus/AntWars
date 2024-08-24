#pragma once

#include <set>
#include <vector>
#include "ant.h"
#include "hill.h"
#include "types.h"

std::vector<HillID> mark_razed_hills(const std::set<Ant,AntComparator>&, const std::set<Hill,HillComparator>&);
