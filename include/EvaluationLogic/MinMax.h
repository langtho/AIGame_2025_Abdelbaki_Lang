//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_MINMAX_H
#define AIGAME_2025_ABDELBAKI_LANG_MINMAX_H
#include "State.h"

class MinMax {
private:
    int max_depth;
    int _minmax(const State& state, int depth, bool player_playing,int alpha,int beta); //true=maximising player
public:
    MinMax(int d):max_depth(d){};
    pair<int,Color> find_best_move(const State& state);
};


#endif //AIGAME_2025_ABDELBAKI_LANG_MINMAX_H