#include "razing_phase.h"
#include <map>

using namespace std;

vector<tuple<HillID, PlayerID>> mark_razed_hills(const AntMap &ants, const std::set<Hill, HillComparator> &hills){
	map<Point, PlayerID> ant_at;
	for(auto &[_, ant] : ants){
		ant_at[ant.get_position()] = ant.get_owner();
	}
	vector<tuple<HillID, PlayerID>> hills_to_remove;
	for(Hill hill : hills){
		Point position = hill.get_position();
		if(ant_at.find(position) != ant_at.end()){
			PlayerID owner_id = ant_at[position];
			if(owner_id != hill.get_owner()) {
				hills_to_remove.push_back({hill.get_id(), owner_id});
				break;
			}
		}
	}
	return hills_to_remove;
}
