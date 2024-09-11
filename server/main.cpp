#include <iostream>
#include "game.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3) {
        cout << "Usage: " << argv[0] << " game_config player_config" << endl;
        return 1;
    }
    auto game_result = Game::load_config(argv[1], argv[2]);
    if(!game_result.has_value()) {
        cout << "Failed to create game from config files" << endl;
        return 1;
    }
    Game game = std::move(game_result.value());
    game.run();
}
