#pragma once

#include <fstream>

class Observer {
public:
    void send(const std::string&);
    Observer(const std::string&, bool = false);
private:
    std::ofstream observation_file;
    bool no_observer;
};
