//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_MINMAX_H
#define AIGAME_2025_ABDELBAKI_LANG_MINMAX_H
#include "State.h"

class MinMax {
private:
    int max_depth;
    // maximizing_player: true if it's the maximizing player's turn in the tree
    int _minmax(const State& state, int depth, bool maximizing_player, bool original_player_is_p1, int alpha, int beta);
public:
    MinMax(int d):max_depth(d){};
    pair<int,Color> find_best_move(const State& state);
};


#endif //AIGAME_2025_ABDELBAKI_LANG_MINMAX_H