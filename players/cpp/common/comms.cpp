#include "comms.h"
#include <sstream>
#include <iostream>

using namespace std;

stringstream read_block(int size) {
    stringstream ret;
    string line;
    for(int i = 0;i < size; i++) {
        if(!getline(cin, line)) {
            cerr << "Error reading block" << endl;
            exit(1);
        }
        if(line == ".") {
            cerr << "Unexpected end of block" << endl;
            exit(1);
        }
        ret << line << "\n";
    }
    return ret;
}

Map parse_map(stringstream& ss) {
    Map ret;
    if(!(ss >> ret.width >> ret.height)) {
        cerr << "Error when parsing map" << endl;
        exit(1);
    }
    ss.ignore(1);
    string line;
    while(getline(ss, line)) {
        Point p;
        stringstream _line(line);
        if(!(_line >> p.x >> p.y)) {
            cerr << "Error when parsing map" << endl;
            exit(1);
        }
        ret.water_tiles.insert(p);
    }

    return ret;
}

tuple<PlayerID, map<HillID, Hill>> parse_spawn_info(stringstream& ss) {
    PlayerID my_id;
    map<HillID, Hill> alive_hills;
    if(!(ss >> my_id)) {
        cerr << "Error when parsing spawn info" << endl;
        exit(1);
    }
    ss.ignore(1);
    string line;
    while(getline(ss, line)) {
        stringstream _line(line);
        Hill hill;
        if(!(_line >> hill.id >> hill.owner >> hill.pos.x >> hill.pos.y)) {
            cerr << "Error when parsing spawn info" << endl;
            exit(1);
        }
        alive_hills[hill.id] = hill;
    }
    return make_tuple(my_id, alive_hills);
}

map<AntID, Ant> parse_alive_ants(stringstream& ss) {
    map<AntID, Ant> ret;
    string line;
    while(getline(ss, line)) {
        Ant ant;
        stringstream _line(line);
        if(!(_line >> ant.id >> ant.owner >> ant.pos.x >> ant.pos.y)) {
            cerr << "Error when parsing alive ants" << endl;
            exit(1);
        }
        ret[ant.id] = ant;
    }
    return ret;
}

set<HillID> parse_alive_hills(stringstream& ss) {
    set<HillID> ret;
    string line;
    while(getline(ss, line)) {
        HillID id;
        stringstream _line(line);
        if(!(_line >> id)) {
            cerr << "Error when parsing alive hills" << endl;
            exit(1);
        }
        ret.insert(id);
    }
    return ret;
}

set<Food> parse_food(stringstream& ss) {
    set<Food> ret;
    string line;
    while(getline(ss, line)) {
        Food f;
        stringstream _line(line);
        if(!(_line >> f.pos.x >> f.pos.y)) {
            cerr << "Error when parsing food" << endl;
            exit(1);
        }
        ret.insert(f);
    }
    return ret;
}

int parse_food_count(stringstream& ss) {
    int ret;
    if(!(ss >> ret)) {
        cerr << "Error when parsing food count" << endl;
        exit(1);
    }
    return ret;
}

void init_world(World &world) {
    string header;
    if(!getline(cin, header)) {
        cerr << "Error when readin init" << endl;
        exit(1);
    }
    if(header != "INIT") {
        cerr << "Wrong header in INIT: " << header << endl;
        exit(1);
    }
    string line;
    while(getline(cin, line)) {
        if(line == ".") {
            break;
        }
        string header;
        int size;
        stringstream ss(line);
        if(!(ss >> header >> size)) {
            cerr << "Invalid block header: " << line << endl;
            exit(1);
        }
        stringstream block = read_block(size);
        if(header == "SPAWN_INFO") {
            tie(world.my_id, world.hills) = parse_spawn_info(block);
        }else if(header == "MAP") {
            world.map = parse_map(block);
        }else {
            cerr << "Unknown block: " << header << endl;
        }
    }
}

void update_world(World &world) {
    string header;
    if(!getline(cin, header)) {
        cerr << "Error when readin init" << endl;
        exit(1);
    }
    int turn;
    stringstream header_stream(header);
    if(!(header_stream >> header >> turn)) {
        cerr << "Invalid header in TURN: " << header << endl;
        exit(1);
    }
    if(header != "TURN") {
        cerr << "Wrong header in TURN: " << header << endl;
        exit(1);
    }
    world.turn = turn;
    string line;
    while(getline(cin, line)) {
        if(line == ".") {
            break;
        }
        string header;
        int size;
        stringstream ss(line);
        if(!(ss >> header >> size)) {
            cerr << "Invalid block header: " << line << endl;
            exit(1);
        }
        stringstream block = read_block(size);
        if(header == "ALIVE_ANTS") {
            world.alive_ants = parse_alive_ants(block);
        }else if(header == "ALIVE_HILLS") {
            world.alive_hills = parse_alive_hills(block);
        }else if(header == "FOOD") {
            world.food = parse_food(block);
        }else if(header == "FOOD_COUNT") {
            world.food_count = parse_food_count(block);
        }else {
            cerr << "Unknown block: " << header << endl;
        }
    }
}
