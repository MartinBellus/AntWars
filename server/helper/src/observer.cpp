#include "observer.h"
#include "format.h"

#include <iostream>

using namespace std;

Observer::Observer(const string& home_dir, bool no_observer) : no_observer(no_observer) {
    observation_file.open(home_dir + "/recording.txt");
    if(!observation_file) {
        cerr << "Error: could not open observation file" << endl;
    }
    score_file.open(home_dir + "/score.txt");
    if(!score_file.is_open()) {
        cerr << "Error: could not open score file" << endl;
    }
}

void Observer::send(const string& data) {
    if(no_observer)
        return;
    observation_file << data;
}

void Observer::save_score(const PlayerMap& players) {
    map<string, int> scores;
    for(const auto& [id, player] : players) {
        scores[player.get_name()] = player.get_score();
    }
    score_file << format::final_score(scores);
}
