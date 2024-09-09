#include "data.h"

using namespace std;

optional<Moves> Moves::from_sstream(stringstream &in) {
	Moves moves;
	string line;
	while(getline(in, line)) {
		stringstream line_stream(line);
		int id, x, y;
		if(cin >> id >> x >> y) {
			moves.data.push_back({AntID(id), Point(x, y)});
		}else {
			// Return None value
			return nullopt;
		}
	}
	return optional(moves);
}
