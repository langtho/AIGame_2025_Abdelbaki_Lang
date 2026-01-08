//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H
#define AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H
#include "State.h"
class Evaluate {
public:
    // maximizing_player_is_p1: true if player 1, false if player 2
    // LEGACY (OldBot) - NICHT ÄNDERN! Das ist unsere Messlatte.
    static int evaluate_state(const State& state, bool maximizing_player_is_p1 = true);
    // CURRENT DEV (NewBot) - Hieran arbeiten wir.
    static int evaluate_stateNEW(const State &state, bool maximizing_player_is_p1);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_EVALUATE_H