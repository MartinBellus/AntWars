#include <map>
#include "gather_phase.h"
#include "ant.h"
#include "food.h"

using namespace std;

vector<pair<FoodID, PlayerID>> get_harvested_food(const std::set<Ant, AntComparator> &ants, const std::set<Food,FoodComparator> &foods){
	map<Point, PlayerID> ant_at;
	for (const Ant &ant : ants){
		ant_at[ant.get_position()] = ant.get_owner();
	}

	vector<pair<FoodID, PlayerID>> harvested_food;
	for(Food food : foods){
		Point position = food.get_position();
		PlayerID owner = PlayerID::NONE;
		for(Point p : points_in_circle(position, constants::ANT_GATHER_RADIUS2)){
			if(ant_at.find(p) != ant_at.end()){
				if(owner == PlayerID::NONE)
					owner = ant_at[p];
				else if(owner != ant_at[p]){
					harvested_food.push_back({food.get_id(), PlayerID::NONE});
					break;
				}
			}
		}

		if(owner != PlayerID::NONE)
			harvested_food.push_back({food.get_id(), owner});
	}

	return harvested_food;
}
