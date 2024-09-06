#pragma once

#include <vector>
#include <set>
#include "types.h"
#include "ant.h"

// @brief Mark all ants that should be killed
// @params ants Set of all ants
// @return Vector of ants, that should be killed
std::vector<AntID> mark_killed_ants(const AntMap&);
