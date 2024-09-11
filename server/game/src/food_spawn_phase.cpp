#include "food_spawn_phase.h"
#include "rng.h"

using namespace std;

vector<Point> get_new_food_locations(int count, const Map& world_map) {
    vector<Point> food_locations;
    for (int i = 0; i < count; ) {
        int x = uniform_int_distribution<>(0, world_map.get_width() - 1)(rng);
        int y = uniform_int_distribution<>(0, world_map.get_height() - 1)(rng);
        if(world_map.can_move_to({x, y})){
            food_locations.push_back({x, y});
            i++;
        }
    }
    return food_locations;
}
