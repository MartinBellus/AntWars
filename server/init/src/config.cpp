#include "config.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

int from_string(const string& value) {
    stringstream ss(value);
    int ret;
    if(!(ss >> ret)) {
        cerr << "Could not parse int: " << value << endl;
        return -1;
    }
    return ret;
}

string at_or(const map<string, string>& dict, const string& key, const string& def) {
    if(dict.find(key) != dict.end()) {
        return dict.at(key);
    }
    return def;
}

optional<GameConfig> GameConfig::parse(const std::string &path) {
    ifstream file(path);
    if(!file.is_open()) {
        cerr << "Could not open file: " << path << endl;
        return nullopt;
    }
    map<string, string> content;
    string line;
    while(getline(file, line)) {
        if(line.size() == 0 || line[0] == '#') {
            continue;
        }
        stringstream ss(line);
        string key, value;
        if(!(ss >> key >> value)) {
            cerr << "Could not parse line: " << line << endl;
            continue;
        }
        content[key] = value;
    }

    GameConfig config;
    config.home_dir = at_or(content, "HOME_DIR", "game");
    config.map_path = at_or(content, "MAP_PATH", "maps/default.ppm");
    config.max_turn_count = from_string(at_or(content, "MAX_TURN_COUNT", "-1"));
    config.max_food_count = from_string(at_or(content, "MAX_FOOD_COUNT", "-1"));
    config.max_player_count = from_string(at_or(content, "MAX_PLAYER_COUNT", "0"));
    config.hills_per_player = from_string(at_or(content, "HILLS_PER_PLAYER", "1"));

    return optional(config);
}

optional<vector<PlayerConfig>> parse_players(const std::string& path){
    ifstream file(path);
    if(!file.is_open()) {
        cerr << "Could not open file: " << path << endl;
        return nullopt;
    }
    vector<PlayerConfig> players;
    string line;
    while(getline(file,line)){
        if(line.size() == 0 || line[0] == '#') {
            continue;
        }
        stringstream ss(line);
        string name, path;
        if(!(ss >> name >> path)) {
            cerr << "Could not parse line: " << line << endl;
            continue;
        }
        players.push_back(PlayerConfig(std::move(name), std::move(path)));
    }

    return optional(players);
}
