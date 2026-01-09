//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H
#define AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H
#include "State.h"
class Evaluate {
public:
    // Static evaluation function.
    // Returns a score from the perspective of the maximizing player (P1 if true, P2 if false).
    static int evaluate_state(const State& state, bool maximizing_player_is_p1 = true);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H