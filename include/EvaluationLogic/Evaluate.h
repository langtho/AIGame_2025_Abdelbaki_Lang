//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H
#define AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H
#include "State.h"
class Evaluate {
public:
    // maximizing_player_is_p1: true if player 1, false if player 2
    static int evaluate_state(const State& state, bool maximizing_player_is_p1 = true);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H