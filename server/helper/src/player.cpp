#include "player.h"

void insert(PlayerMap& m, Player&& p) {
	m[p.get_id()] = std::move(p);
}
