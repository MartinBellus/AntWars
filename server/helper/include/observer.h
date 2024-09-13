#pragma once

#include <fstream>
#include "player.h"

class Observer {
public:
    void send(const std::string&);
    void save_score(const PlayerMap&);
    Observer(const std::string&, bool = false);
private:
    std::ofstream observation_file;
    std::ofstream score_file;
    bool no_observer;
};
