#include "ant.h"

void insert(AntMap& m, Ant&& ant) {
	m[ant.get_id()] = std::move(ant);
}
