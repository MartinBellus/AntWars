#include "check_collisions.h"
#include <map>

using namespace std;

set<AntID> get_colliding_ants(const AntMap &ants) {
    set<AntID> ret;
    map<Point, AntID> ant_at;
    for(const auto& [ant_id, ant] : ants) {
        if(ant_at.find(ant.get_position()) != ant_at.end()) {
            ret.insert(ant_id);
            ret.insert(ant_at[ant.get_position()]);
        }else {
            ant_at[ant.get_position()] = ant_id;
        }
    }
    return ret;
}
