#include "game.h"
#include "config.h"
#include "player_manager.h"
#include "rng.h"
#include "score.h"

#include <algorithm>
#include <sys/stat.h>

using namespace std;

int setup_dirs(const char* home_dir) {
    if(mkdir(home_dir, 0775) != 0 && errno != EEXIST) {
        cerr << "Failed to create home directory" << endl;
        return 1;
    }
    char logs_dir[1024];
    snprintf(logs_dir, 1024, "%s/logs", home_dir);
    if(mkdir(logs_dir, 0775) != 0 && errno != EEXIST) {
        cerr << "Failed to create logs directory" << endl;
        return 1;
    }
    return 0;
}

optional<Game> Game::load_config(const std::string& game_config_path, const std::string& player_config_path) {
    auto game_config_result = GameConfig::parse(game_config_path);
    if(!game_config_result.has_value()) {
        return nullopt;
    }
    GameConfig game_config = std::move(game_config_result.value());

    auto player_config_result = parse_players(player_config_path);
    if(!player_config_result.has_value()) {
        return nullopt;
    }
    vector<PlayerConfig> player_configs = std::move(player_config_result.value());

    if(game_config.max_player_count < player_configs.size()) {
        cerr << "Too many players in player_config, max: " << game_config.max_player_count << endl;
        return nullopt;
    }

    // set constants
    if(game_config.max_food_count != -1) {
        constants::MAX_FOOD_COUNT = game_config.max_food_count;
    }
    if(game_config.max_turn_count != -1) {
        constants::MAX_TURN_COUNT = game_config.max_turn_count;
    }

    // create folder structure
    if(setup_dirs(game_config.home_dir.c_str()) != 0) {
        return nullopt;
    }

    // setup observer
    Observer observer(game_config.home_dir, game_config.no_observer);

    // setup logger
    Logger logger(game_config.home_dir + "/logs/__server.txt");

    // setup map
    auto map_result = Map::from_ppm(game_config.map_path);
    if(!map_result.has_value()) {
        return nullopt;
    }
    Map world_map = std::move(map_result.value());

    // setup player manager
    vector<ProbojPlayerConfig> proboj_configs;
    for(const PlayerConfig& conf : player_configs) {
        proboj_configs.push_back(ProbojPlayerConfig{conf, Limits{}}); // TODO default limits for now
    }
    PlayerManager player_manager(game_config.home_dir, proboj_configs);

    Game game(
        std::move(world_map),
        std::move(player_manager),
        std::move(logger),
        std::move(observer)
    );

    int player_count = player_configs.size();

    // choose starting hills
    int used_hills = game_config.hills_per_player*player_count;
    if(used_hills > game.world_map.get_hill_positions().size()) {
        cerr << "Not enough hills for all players" << endl;
        return nullopt;
    }
    vector<Point> selected_hills;
    sample(game.world_map.get_hill_positions().begin(),
        game.world_map.get_hill_positions().end(),
        back_inserter(selected_hills), used_hills, rng);
    shuffle(selected_hills.begin(), selected_hills.end(), rng);

    int current_hill = 0;
    // setup alive players
    for(const PlayerConfig& conf : player_configs) {
        Player player = Player(conf.name, conf.id);
        for(int i = 0; i < game_config.hills_per_player;i++,current_hill++) {
            Point pos = selected_hills[current_hill];
            // assign new hill and ant to player
            Hill hill(pos, conf.id);
            game.player_hills[conf.id].insert(hill.get_id());
            game.alive_hills.insert(std::move(hill));
            insert(game.alive_ants, Ant(pos, conf.id));
            player.inc_ants();
            player.update_score(score::POINTS_PER_HILL);
        }
        game.alive_players.insert(player.get_id());
        insert(game.all_players, std::move(player));
    }

    return game;
}
