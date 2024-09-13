#include "common/bot.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void Bot::init(const World& world) {
    cerr << "This appears in the log file" << endl;
    return;
}

vector<Point> directions = {
    {0, 1},
    {1, 0},
    {0, -1},
    {-1, 0}
};

vector<Move> Bot::get_turn(const World& world) {
    vector<Move> moves;
    for(auto &[id, ant] : world.alive_ants) {
        if(ant.owner == world.my_id) {
            moves.push_back({id, ant.pos + directions[rand() % 4]});
        }
    }
    return moves;
}
