//
// Created by loko on 31/10/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_GAME_H
#define AIGAME_2025_ABDELBAKI_LANG_GAME_H
#include "State.h"
#include "Color.h"

class Game {
    private:
    State currentState;

    public:
    Game():currentState(State(Board(),0,0,true)){}

    void runAIBattle();
    void run();
};

#endif //AIGAME_2025_ABDELBAKI_LANG_GAME_H