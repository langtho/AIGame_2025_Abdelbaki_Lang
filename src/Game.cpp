//
// Created by loko on 31/10/2025.
//

#include "Game.h"
#include "GameRules.h"
#include "EvaluationLogic/MinMax.h"
#include "MCTS.h"
#include "Bot.h"
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

void Game::run() {
    // Choose mode
    int modeChoice;
    std::cout << "Choose game mode:" << std::endl;
    std::cout << "0: PVP (Player vs Player)" << std::endl;
    std::cout << "1: PVE (Player vs AI)" << std::endl;
    std::cout << "2: EVE (AI vs AI)" << std::endl;
    std::cin >> modeChoice;
    mode = static_cast<GameMode>(modeChoice);

    Bot* bot1 = nullptr;
    Bot* bot2 = nullptr;

    int limit = 0;

	if (mode == PVE || mode == EVE) {
        bot2 = new Bot(4); 
    }
    if (mode == EVE) {
        bot1 = new Bot(4);
    }

    bool gameOver = false;
    while (!gameOver) {
        if(limit++>400){
            std::cout<<"Max moves played (400)"<<std::endl;
            gameOver=true;
            break;
        }
        std::cout << "Score P1: " << currentState.score_p1 << " Score P2: " << currentState.score_p2 << std::endl;
        currentState.board.showBoard();

        std::pair<int, Color> move;
        if ((currentState.player_playing && mode == EVE) || (!currentState.player_playing && (mode == PVE || mode == EVE))) {
            // AI turn
            Bot* currentBot = currentState.player_playing ? bot1 : bot2;
            
            auto start_time = std::chrono::high_resolution_clock::now();
            move = currentBot->getMove(currentState);
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            
            if (GameRules::getPossibleMoves(currentState).empty()) {
                std::cout << "No possible moves, game over." << std::endl;
                gameOver = true;
                break;
            }
            std::cout << "AI chooses " << move.first + 1 << " " << move.second << " (" << duration.count() << "ms)" << std::endl;
        } else {
            // Human turn
            int field, color;
            bool validField = false, validColor = false;

            std::cout << "Choose your Field (1-16): " << std::endl;
            while (!validField) {
                std::cin >> field;
                if (field >= 1 && field <= 16) {
                    bool isPlayerHole = currentState.player_playing ? ((field - 1) % 2 == 0) : ((field - 1) % 2 == 1);
                    if (isPlayerHole) {
                        validField = true;
                        field--; // 0-based
                    } else {
                        std::cout << "Invalid field (not your hole), try again: " << std::endl;
                    }
                } else {
                    std::cout << "Invalid field, try again: " << std::endl;
                }
            }

            std::cout << "Choose your Color: " << std::endl;
            std::cout << "0: red" << std::endl;
            std::cout << "1: blue" << std::endl;
            std::cout << "2: transparent red" << std::endl;
            std::cout << "3: transparent blue" << std::endl;
            while (!validColor) {
                std::cin >> color;
                if (color >= 0 && color <= 3) {
                    validColor = true;
                } else {
                    std::cout << "Invalid color, try again: " << std::endl;
                }
            }
            move = {field, static_cast<Color>(color)};
        }

        currentState = GameRules::playMove(currentState, move.first, move.second);
        gameOver = GameRules::gameOver(currentState);
    }

    std::cout << "Game over! P1: " << currentState.score_p1 << " P2: " << currentState.score_p2 << std::endl;

    delete bot1;
    delete bot2;
}

// src/Game.cpp


void Game::runAIBattle() {
    GameRules game_rules;
    bool gameOver = false;
    bool player1turn = true;

    Game game;

    int mcts_iterations = 1000;
    int minmax_depth = 10;

    MCTS mcts_ai(mcts_iterations);
    mcts_ai.initialize(game.currentState);

    MinMax minmax_ai(minmax_depth);

    pair<int, Color> move;

    cout << "=== AI BATTLE MODE STARTED ===" << endl;
    cout << "P1: MCTS (" << mcts_iterations << " iters) vs P2: MinMax (Depth " << minmax_depth << ")" << endl;
    cout << "Format: [MoveNr] Player : Field | Color -> Score P1:P2" << endl;
    cout << "--------------------------------------------------------" << endl;

    while (!gameOver) {

        auto start_time = chrono::high_resolution_clock::now();
        
        if (player1turn) {
            move = mcts_ai.find_best_move(game.currentState);
        } else {
            move = minmax_ai.find_best_move(game.currentState, std::chrono::milliseconds(3000));
        }
        
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

        string colorStr;
        switch (move.second) {
            case red: colorStr = "R"; break;
            case blue: colorStr = "B"; break;
            case transparentRED: colorStr = "TR"; break;
            case transparentBLUE: colorStr = "TB"; break;
        }

        cout << "[" << game.currentState.moves_played + 1 << "] " << (player1turn ? "MinMax" : "MCTS  ") << " : " << move.first + 1 << colorStr << " (" << duration.count() << "ms) ";

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

string moveToString(const pair<int, Color>& move) {
    string colorStr;
    switch (move.second) {
        case red: colorStr = "R"; break;
        case blue: colorStr = "B"; break;
        case transparentRED: colorStr = "TR"; break;
        case transparentBLUE: colorStr = "TB"; break;
    }
    return to_string(move.first + 1) + colorStr;
}

pair<int, Color> parseMove(const string& moveStr) {
    int field = 0;
    Color color = red;
    
    size_t i = 0;
    while (i < moveStr.length() && isdigit(moveStr[i])) {
        i++;
    }
    
    field = stoi(moveStr.substr(0, i)) - 1;
    
    string colorStr = moveStr.substr(i);
    if (colorStr == "R") color = red;
    else if (colorStr == "B") color = blue;
    else if (colorStr == "TR") color = transparentRED;
    else if (colorStr == "TB") color = transparentBLUE;
    
    return {field, color};
}

void Game::runAsExternalPlayer() {
    int minmax_depth = 8;
    MinMax ai(minmax_depth);
    
    State state(Board(), 0, 0, true);
    
    string input;
    while (getline(cin, input)) {
        if (input == "END") {
            break;
        }
        
        pair<int, Color> ourMove;
        
        if (input == "START") {
            state = State(Board(), 0, 0, true);
            
            ourMove = ai.find_best_move(state, chrono::milliseconds(2500));
            state = GameRules::playMove(state, ourMove.first, ourMove.second);
            
            cout << moveToString(ourMove) << endl;
            cout.flush();
        }
        else if (input.find("RESULT") != string::npos) {
            break;
        }
        else {
            if (state.moves_played == 0) {
                state = State(Board(), 0, 0, true);
            }
            
            pair<int, Color> oppMove = parseMove(input);
            state = GameRules::playMove(state, oppMove.first, oppMove.second);
            
            if (GameRules::gameOver(state)) {
                cout << "RESULT " << moveToString(oppMove) << " " << state.score_p1 << " " << state.score_p2 << endl;
                cout.flush();
                break;
            }
            
            ourMove = ai.find_best_move(state, chrono::milliseconds(2500));
            state = GameRules::playMove(state, ourMove.first, ourMove.second);
            
            if (GameRules::gameOver(state)) {
                cout << "RESULT " << moveToString(ourMove) << " " << state.score_p1 << " " << state.score_p2 << endl;
                cout.flush();
                break;
            }
            
            cout << moveToString(ourMove) << endl;
            cout.flush();
        }
    }
}