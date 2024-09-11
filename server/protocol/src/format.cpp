#include "format.h"
#include "food.h"
#include "hill.h"

#include <sstream>

using namespace std;

const char DOT = '.';

string format_point(const Point& p) {
    return to_string(p.x) + " " + to_string(p.y);
}

namespace observer {
    string format_spawn_points(const std::set<Hill,HillComparator> hills) {
        stringstream ret;
        ret << "SPAWN_POSITIONS " << hills.size() << "\n";
        for(const Hill& hill : hills) {
            ret << str(hill.get_id()) << " "
            << str(hill.get_owner()) <<  " "
            << format_point(hill.get_position()) << "\n";
        }
        return ret.str();
    }

    string format_player_names(const PlayerMap& players) {
        stringstream ret;
        ret << "PLAYER_NAMES " << players.size() << "\n";
        for(const auto &[id, player]: players) {
            ret << str(player.get_id()) << " "
            << player.get_name() << "\n";
        }
        return ret.str();
    }
}

string format_alive_ants(const AntMap& alive_ants) {
    stringstream ret;
    ret << "ALIVE_ANTS " << alive_ants.size() << "\n";
    for(const auto &[id, ant] : alive_ants) {
        ret << str(ant.get_owner()) << " "
        << str(ant.get_id()) << " "
        << format_point(ant.get_position()) << "\n";
    }

    return ret.str();
}

string format_alive_hills(const std::set<Hill, HillComparator>& hills) {
    stringstream ret;
    ret << "ALIVE_HILLS " << hills.size() << "\n";
    for(const Hill& hill : hills) {
        ret << str(hill.get_id()) << "\n";
    }
    return ret.str();
}

string format_alive_food(const std::set<Food, FoodComparator>& food) {
    stringstream ret;
    ret << "FOOD " << food.size() << "\n";
    for(const Food& f : food) {
        ret << format_point(f.get_position()) << "\n";
    }
    return ret.str();
}

string format_map(const Map& map) {
    stringstream ret;

    ret << "MAP " << map.get_water_tiles().size()+1 << "\n"
    << map.get_width() << " " << map.get_height() << "\n";
    for(const Point& p : map.get_water_tiles()) {
        ret << format_point(p) << "\n";
    }

    return ret.str();
}

namespace client {
    string format_food_count(int food_count) {
        return "FOOD_COUNT 1\n" + to_string(food_count) + "\n";
    }
    string format_spawn_info(PlayerID my_id, const std::set<Hill, HillComparator>& hills) {
        stringstream ret;
        ret << "SPAWN_INFO " << hills.size() + 1 << "\n"
        << str(my_id) << "\n";
        for(const Hill& hill : hills) {
            ret << str(hill.get_id()) << " "
            << str(hill.get_owner()) << " "
            << format_point(hill.get_position()) << "\n";
        }
        return ret.str();
    }
}

namespace format {
    string to_observer(const ObserverTurn& turn) {
        stringstream ret;
        ret << "TURN " << turn.turn_num << "\n"
        << format_alive_ants(turn.alive_ants)
        << format_alive_hills(turn.alive_hills)
        << format_alive_food(turn.alive_food)
        << DOT << "\n";
        return ret.str();
    }

    string init_observer(const ObserverInit& init) {
        stringstream ret;
        ret << "INIT\n"
        << observer::format_player_names(init.players)
        << observer::format_spawn_points(init.spawn_positions)
        << format_map(init.map)
        << DOT << "\n";

        return ret.str();
    }

    string to_player(const PlayerTurn& turn) {
        stringstream ret;
        ret << "TURN " << turn.turn_num << "\n"
        << format_alive_ants(turn.alive_ants)
        << format_alive_hills(turn.alive_hills)
        << format_alive_food(turn.alive_food)
        << client::format_food_count(turn.food_count)
        << DOT << "\n";
        return ret.str();
    }

    string init_player(const PlayerInit& init) {
        stringstream ret;
        ret << "INIT\n"
        << client::format_spawn_info(init.my_id, init.spawn_positions)
        << format_map(init.map)
        << DOT << "\n";
        return ret.str();
    }

}
