//
// Created by loko on 31/10/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_GAME_H
#define AIGAME_2025_ABDELBAKI_LANG_GAME_H
#include "State.h"
#include "Color.h"

enum GameMode {
    PVP, // Player vs Player
    PVE, // Player vs AI
    EVE  // AI vs AI
};

class Game {
    private:
    State currentState;
    GameMode mode;

    public:
    Game():currentState(State(Board(),0,0,true)), mode(PVP){}

    void runAIBattle();
    void runStrategyBattle();
    void runCompetition();
    void runCompetitionNoPondering(bool isJoueurA);
    //void runCompetitionNoPondering();
    void run();
    void setMode(GameMode m) { mode = m; }
};

#endif //AIGAME_2025_ABDELBAKI_LANG_GAME_H