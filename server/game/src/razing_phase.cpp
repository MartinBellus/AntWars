#include "razing_phase.h"
#include <map>

using namespace std;

vector<HillID> mark_razed_hills(const std::set<Ant, AntComparator> &ants, const std::set<Hill, HillComparator> &hills){
	map<Point, AntID> ant_at;
	for(const Ant& ant : ants){
		ant_at[ant.get_position()] = ant.get_id();
	}
	vector<HillID> hills_to_remove;
	for(Hill hill : hills){
		Point position = hill.get_position();
		if(ant_at.find(position) != ant_at.end()){
			Ant ant = *ants.find(ant_at[position]);
			hills_to_remove.push_back(hill.get_id());
		}
	}
	return hills_to_remove;
}
