#include "observer.h"

#include <iostream>

using namespace std;

Observer::Observer(const string& observation_file_path, bool no_observer) : no_observer(no_observer) {
    observation_file.open(observation_file_path);
    if(!observation_file) {
        cerr << "Error: could not open observation file" << endl;
    }
}

void Observer::send(const string& data) {
    if(no_observer)
        return;
    observation_file << data;
}
