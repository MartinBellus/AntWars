#pragma once

#include "types.h"
#include <map>
#include <optional>
#include <string>
#include <vector>

struct GameConfig {
    std::string map_path; // optional
    std::string home_dir; // optional
    int max_turn_count; // optional
    int max_food_count; // optional
    int max_player_count;
    int hills_per_player; // optional
    bool no_observer = false;
    bool no_logger = false;
    static std::optional<GameConfig> parse(const std::string&);
};

struct PlayerConfig {
    PlayerConfig(std::string name, std::string dir_path) : name(name), dir_path(dir_path), id(++id_counter) {}
    std::string name;
    std::string dir_path;
    bool use_sandbox = true;
    PlayerID id;
    static inline PlayerID id_counter = PlayerID(0);
};

std::optional<std::vector<PlayerConfig>> parse_players(const std::string&);
