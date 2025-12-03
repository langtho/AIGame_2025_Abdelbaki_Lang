//
// Created by loko on 31/10/2025.
//

#include "Game.h"
#include "GameRules.h"
#include "EvaluationLogic/MinMax.h"
#include "MCTS.h"
void Game::run() {


        MinMax ai_MINMAX(7);
        MCTS ai_MCTS=MCTS(1000);
        pair<int,Color> best_move;
        GameRules game_rules;
        bool gameOver=false;
        bool player1turn=true;
        Game game;
        ai_MCTS.initialize(game.currentState);
        while (!gameOver) {
            cout<<"Score P1: "<<game.currentState.score_p1<<"Score P2: "<<game.currentState.score_p2<<endl;
            game.currentState.board.showBoard();
            if (player1turn) {
                cout<<"Player1 turn"<<endl;
                best_move=ai_MCTS.find_best_move(game.currentState);
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

            ai_MCTS.advance_tree({field,(Color)color}, game.currentState);
            gameOver=game_rules.gameOver(game.currentState);
            player1turn=!player1turn;

        }

        cout<<"Game is over P1: "<<game.currentState.score_p1<<"Score P2: "<<game.currentState.score_p2<<endl;

}

// src/Game.cpp


void Game::runAIBattle() {
    GameRules game_rules;
    bool gameOver = false;
    bool player1turn = true;

    Game game;

    int mcts_iterations = 3000;
    int minmax_depth = 6;

    MCTS mcts_ai(mcts_iterations);
    mcts_ai.initialize(game.currentState);

    MinMax minmax_ai(minmax_depth);

    pair<int, Color> move;

    cout << "=== AI BATTLE MODE STARTED ===" << endl;
    cout << "P1: MCTS (" << mcts_iterations << " iters) vs P2: MinMax (Depth " << minmax_depth << ")" << endl;
    cout << "Format: [MoveNr] Player : Field | Color -> Score P1:P2" << endl;
    cout << "--------------------------------------------------------" << endl;

    while (!gameOver) {

        if (player1turn) {

            move = mcts_ai.find_best_move(game.currentState);
        } else {

            move = minmax_ai.find_best_move(game.currentState);
        }


        cout << "[" << game.currentState.moves_played + 1 << "] "
             << (player1turn ? "MCTS  " : "MinMax") << " : "
             << move.first + 1 << " | " << move.second << " ";


        game.currentState = game_rules.playMove(game.currentState, move.first, move.second);


        mcts_ai.advance_tree(move, game.currentState);


        cout << "-> " << game.currentState.score_p1 << ":" << game.currentState.score_p2 << endl;


        gameOver = game_rules.gameOver(game.currentState);
        player1turn = !player1turn;
    }


    cout << "--------------------------------------------------------" << endl;
    cout << "BATTLE FINISHED after " << game.currentState.moves_played << " moves." << endl;
    cout << "Final Score: MCTS " << game.currentState.score_p1 << " - " << game.currentState.score_p2 << " MinMax" << endl;

    if (game.currentState.score_p1 > game.currentState.score_p2)
        cout << "WINNER: MCTS" << endl;
    else if (game.currentState.score_p2 > game.currentState.score_p1)
        cout << "WINNER: MinMax" << endl;
    else
        cout << "RESULT: DRAW" << endl;

    cout << "\nFinal Board State:" << endl;
    game.currentState.board.showBoard();
}