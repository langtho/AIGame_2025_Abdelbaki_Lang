//
// Created by loko on 04/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_STATE_H
#define AIGAME_2025_ABDELBAKI_LANG_STATE_H
#include <vector>
#include "Board.h"
using namespace std;

class State {
    public:
    Board board;
    int score_p1, score_p2;
    bool player_playing;
    int moves_played;


    State(const Board& b, int p1,int p2, bool pp):board(b),score_p1(p1),score_p2(p2),player_playing(pp){};
    State(const State& other)=default;

    void afficherState(int field,Color c,int dep) const{
        cout<<dep;
        cout<<"-------------------STATE--"<<field<<"|"<<c<<"-------------------------------"<<endl;
        cout<<"Score P1: "<<score_p1<<"Score P2: "<<score_p2<<endl;
        board.showBoard();
        cout<<"---------------------------------------------------------"<<endl;
    };
};

#endif //AIGAME_2025_ABDELBAKI_LANG_STATE_H