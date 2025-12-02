//
// Created by loko on 31/10/2025.
//

#include "Game.h"
#include "GameRules.h"
#include "EvaluationLogic/MinMax.h"
void Game::run() {


        MinMax ai(6);
        pair<int,Color> best_move;

        GameRules game_rules;
        bool gameOver=false;
        bool player1turn=true;
        Game game;
        while (!gameOver) {
            cout<<"Score P1: "<<game.currentState.score_p1<<"Score P2: "<<game.currentState.score_p2<<endl;
            game.currentState.board.showBoard();
            if (player1turn) {
                cout<<"Player1 turn"<<endl;
                best_move=ai.find_best_move(game.currentState);
                cout<<"The AI chooses"<<best_move.first+1<<"||"<<best_move.second<<endl;
            }else {
                cout<<"Player2 turn"<<endl;
            }
            int field;
            int color;
            bool isfield=false,isSeed=false;

            cout<<"Choose your Field: "<<endl;
            while (isfield==false) {
                cin>>field;
                if (field>0 && field<=16) {
                    isfield=true;
                }else {
                    cout<<"Not a valid field try again"<<endl;
                }
            }

            field--;
            cout<<"Choose your Color: "<<endl<<"0:red"<<endl<<"1:blue"<<endl<<"2:transparent red"<<endl<<"3:transparent blue"<<endl;
            while (isSeed==false) {
                cin>>color;
                if (color>=0 && color<=3) {
                    isSeed=true;
                }else {
                    cout<<"Not a valid Seed ID try again"<<endl;
                }
            }
            game.currentState=game_rules.playMove(game.currentState,field,(Color)color);


            gameOver=game_rules.gameOver(game.currentState);
            player1turn=!player1turn;

        }

        cout<<"Game is over P1: "<<game.currentState.score_p1<<"Score P2: "<<game.currentState.score_p2<<endl;

}

