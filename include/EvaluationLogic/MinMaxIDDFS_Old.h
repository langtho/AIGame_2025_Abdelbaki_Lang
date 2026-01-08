#ifndef AIGAME_2025_ABDELBAKI_LANG_MINMAXIDDFS_OLD_H
#define AIGAME_2025_ABDELBAKI_LANG_MINMAXIDDFS_OLD_H

#include "../State.h"
#include "MinMaxIDDFS.h" // Für TTEntry Struktur
#include <unordered_map>
#include <chrono>
#include <vector>

class MinMaxIDDFS_Old {
private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    int time_limit_ms;
    bool time_out;

    std::unordered_map<uint64_t, TTEntry> tt;

    static bool z_initialized;
    static uint64_t z_red[16][100];
    static uint64_t z_blue[16][100];
    static uint64_t z_trans[16][100];
    static uint64_t z_turn;

    void initZobrist();
    uint64_t computeHash(const State& state);

    int _minmax(const State& state, int depth, int alpha, int beta, bool maximizing_player, bool original_player_is_p1);

public:
    MinMaxIDDFS_Old(int time_ms = 1850);
    std::pair<int, Color> find_best_move(const State& state);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_MINMAXIDDFS_OLD_H