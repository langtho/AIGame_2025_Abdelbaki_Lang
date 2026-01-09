//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_MINMAX_H
#define AIGAME_2025_ABDELBAKI_LANG_MINMAX_H
#include "State.h"
#include <chrono>

class MinMax {
private:
    int max_depth;
    bool timeout_reached;
    // maximizing_player: true if it's the maximizing player's turn in the tree
    int _minmax(const State& state, int depth, bool maximizing_player, bool original_player_is_p1, int alpha, int beta,
                const std::chrono::steady_clock::time_point& start_time, std::chrono::milliseconds timeout);
public:
    MinMax(int d):max_depth(d), timeout_reached(false){};
    pair<int,Color> find_best_move(const State& state, std::chrono::milliseconds timeout = std::chrono::milliseconds(3000));
};


#endif //AIGAME_2025_ABDELBAKI_LANG_MINMAX_H