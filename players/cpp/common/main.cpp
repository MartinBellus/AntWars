#include "bot.h"
#include "comms.h"
#include <iostream>

using namespace std;

int main() {
    Bot player_bot;
    World world;
    init_world(world);
    player_bot.init(world);
    cout << '.' << endl;
    while(1) {
        update_world(world);
        vector<Move> moves = player_bot.get_turn(world);
        for(Move move : moves) {
            cout << move.ant << " " << move.to.x << " " << move.to.y << "\n";
        }
        cout << '.' << endl;
    }
}
