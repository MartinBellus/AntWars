#pragma once

#include <vector>
#include "data.h"
#include "constants.h"

class Bot {
public:
    ///@brief Runs once before game starts
    ///@param world - partially initialized world
    void init(const World&);

    ///@brief Runs every turn
    ///@param world - current game state
    ///@return vector of ant moves
    std::vector<Move> get_turn(const World&);
};
