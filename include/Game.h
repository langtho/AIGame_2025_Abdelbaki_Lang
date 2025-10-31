//
// Created by loko on 31/10/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_GAME_H
#define AIGAME_2025_ABDELBAKI_LANG_GAME_H
#include "Board.h"
#include "Color.h"

class Game {
    private:
    Board* board;
    int score_p1,score_p2;

    public:
    Game():board(new Board),score_p1(0),score_p2(0){}
    ~Game(){delete board;}

    Board* getBoard();
    int getScoreP1();
    int getScoreP2();

    void setScoreP1(int score);
    void setScoreP2(int score);

    bool gameOver();
    int playMove(int field,Color color);
    int capture(int field, bool everyField);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_GAME_H