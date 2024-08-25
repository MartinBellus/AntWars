#include "data.h"

using namespace std;

// TODO: Return expected<Moves>
Moves Moves::from_sstream(stringstream &in) {
	Moves moves;
	string line;
	while(getline(in, line)) {
		stringstream line_stream(line);
		int id, x, y;
		if(cin >> id >> x >> y) {
			moves.data.push_back({AntID(id), Point(x, y)});
		}else {
			// TODO: Log parsing error
		}
	}
	return moves;
}
