//
// Created by loko on 31/10/2025.
//

#include "Game.h"
#include "GameRules.h"
#include "Bot.h"
#include <iostream>

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

    /*
    * depth = 2 yielded okay results in testing (Game over! P1: 42 P2: 48)
    * depth = 3 yielded bad results in testing (infinite loop)
    * depth = 4 yielded good results in testing (Game over! P1: 43 P2: 47)
    * depth = 5 yielded bad results in testing (infinite loop)
    * depth = 6 yielded good results in testing (Game over! P1: 48 P2: 39)
    */

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
            move = currentBot->getMove(currentState);
            if (GameRules::getPossibleMoves(currentState).empty()) {
                std::cout << "No possible moves, game over." << std::endl;
                gameOver = true;
                break;
            }
            std::cout << "AI chooses " << move.first + 1 << " " << move.second << std::endl;
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

