//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_GAMERULES_H
#define AIGAME_2025_ABDELBAKI_LANG_GAMERULES_H
#include "Color.h"
#include "State.h"

class GameRules {
    public:
    static State playMove(const State& state,int field,Color color);
    static vector<pair<int,Color>> getPossibleMoves(const State& state);
    static bool gameOver(const State& state);
    static int capture( State& state,int field);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_GAMERULES_H