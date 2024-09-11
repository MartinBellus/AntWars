#include "ant_spawn_phase.h"
#include <algorithm>

using namespace std;

vector<Point> get_new_ant_pos(const set<HillID>& my_hills, const set<Hill, HillComparator>& alive_hills, const AntMap& ants) {
    set<Point> occupied;
    for (const auto& [id, ant] : ants) {
        occupied.insert(ant.get_position());
    }
    vector<Point> possible_positions;
    for(HillID hill_id : my_hills) {
        auto hill = alive_hills.find(hill_id);
        if(occupied.find(hill->get_position()) == occupied.end()) {
            possible_positions.push_back(hill->get_position());
        }
    }
    shuffle(possible_positions.begin(), possible_positions.end(), rand);
    return possible_positions;
}
