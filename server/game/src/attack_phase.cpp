#include "attack_phase.h"
#include "ant.h"
#include "point.h"

#include <map>

using namespace std;

bool are_battling(const Ant& a, const Ant& b){
	return (a.get_position() - b.get_position()).dist2() <= constants::ANT_BATTLE_RADIUS2 && a.get_owner() != b.get_owner();
}

vector<AntID> mark_killed_ants(const set<Ant, AntComparator>& ants){
	map<AntID, int> focus_split;
	vector<AntID> killed_ants;

	map<Point, AntID> ant_at;

	for(const Ant &a : ants) {
		ant_at[a.get_position()] = a.get_id();
	}

	for(const Ant &a : ants) {
		for(Point p : points_in_circle(a.get_position(), constants::ANT_BATTLE_RADIUS2)){
			if(ant_at.find(p) == ant_at.end()) continue;
			Ant b = *ants.find(ant_at[p]);
			if(are_battling(a, b)){
				focus_split[a.get_id()]++;
			}
		}
	}

	for(const Ant &a : ants) {
		for(Point p : points_in_circle(a.get_position(), constants::ANT_BATTLE_RADIUS2)){
			if(ant_at.find(p) == ant_at.end()) continue;
			Ant b = *ants.find(ant_at[p]);
			AntID a_id = a.get_id(), b_id = b.get_id();
			if(a_id == b_id) continue;
			if(are_battling(a, b)){
				if(focus_split[a_id] >= focus_split[b_id]){
					killed_ants.push_back(a_id);
					break;
				}
			}
		}
	}

	return killed_ants;
}
