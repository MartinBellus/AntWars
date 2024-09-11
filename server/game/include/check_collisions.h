#pragma once

#include <set>

#include "types.h"
#include "ant.h"

std::set<AntID> get_colliding_ants(const AntMap&);
