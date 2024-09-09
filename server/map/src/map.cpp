#include "map.h"
#include "parse_ppm.h"
#include <map>

using namespace std;

enum class Tile {
    EMPTY,
    WATER,
    HILL,
};

const std::map<PpmPixel, Tile> pixel_to_tile = {
    {{255, 255, 255}, Tile::EMPTY},
    {{0, 0, 255}, Tile::WATER},
    {{255, 0, 0}, Tile::HILL}
};


optional<Map> Map::from_ppm(const std::string& image_path) {
    auto parsed_data = parse_ppm(image_path);
    if (!parsed_data.has_value()) {
        return nullopt;
    }
    auto [header, data] = parsed_data.value();

    Map ret(header.width, header.height);
    for(int y = 0;y < header.height;y++){
        for(int x = 0;x < header.width;x++){
            PpmPixel pixel = data[x + y * header.width];
            if(pixel_to_tile.find(pixel) == pixel_to_tile.end()){
                cerr << "Unknown pixel color in position: " << x << " " << y << endl;
                return nullopt;
            }
            Tile tile = pixel_to_tile.at(pixel);
            if(tile == Tile::WATER){
                ret.water_tiles.insert(Point(x, y));
            }
            if(tile == Tile::HILL) {
                ret.hill_positions.insert(Point(x, y));
            }
        }
    }
    return optional(ret);
}

bool Map::inside(const Point& to) const {
    return to.x >= 0 && to.x < width && to.y >= 0 && to.y < height;
}

bool Map::can_move_to(const Point& to) const {
    return inside(to) && water_tiles.find(to) == water_tiles.end();
}
