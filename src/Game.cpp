//
// Created by loko on 31/10/2025.
//

#include "Game.h"
#include "GameRules.h"
#include "EvaluationLogic/MinMax.h"
#include "MCTS.h"
#include "Bot.h"
#include "BotIDDFS.h"
#include "BotIDDFS_Old.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>

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

// src/Game.cpp


void Game::runAIBattle() {
    // Configuration
    int num_games=100;

    int mcts_iterations = 5000; // 5000 si Debug, 25000-50000 si Release
    int minmax_depth = 6;

    // Stats globales
    int mcts_wins = 0;
    int minmax_wins = 0;
    int draws = 0;

    // Initialisation des IAs (une seule fois pour garder le cache/mémoire !)
    MCTS mcts_ai(mcts_iterations);
    mcts_ai.loadKnowledge("final_games.txt"); // Chargement du cerveau binaire

    MinMax minmax_ai(minmax_depth);
    GameRules game_rules;

    cout << "\n=== AI BATTLE TOURNAMENT STARTED ===" << endl;
    cout << "MCTS (" << mcts_iterations << " iters) vs MinMax (Depth " << minmax_depth << ")" << endl;
    cout << "Total Games: " << num_games << endl;
    cout << "--------------------------------------------------------" << endl;

    for (int g = 1; g <= num_games; g++) {
        Game game;
        bool gameOver = false;

        bool mcts_is_p1 = (g % 2 != 0);

        mcts_ai.initialize(game.currentState);

        cout << "\nGame " << g << " / " << num_games << " Started!" << endl;
        if (mcts_is_p1) cout << ">> P1: MCTS (Starts) vs P2: MinMax" << endl;
        else            cout << ">> P1: MinMax (Starts) vs P2: MCTS" << endl;

        while (!gameOver) {
            pair<int, Color> move;


            bool is_p1_turn = game.currentState.player_playing;

            bool is_mcts_turn = (mcts_is_p1 && is_p1_turn) || (!mcts_is_p1 && !is_p1_turn);

            string player_name = is_mcts_turn ? "MCTS  " : "MinMax";

            // --- CHRONO START ---
            auto start_time = std::chrono::high_resolution_clock::now();

            if (is_mcts_turn) {
                move = mcts_ai.find_best_move(game.currentState);
            } else {
                move = minmax_ai.find_best_move(game.currentState);
            }

            // --- CHRONO END ---
            auto end_time = std::chrono::high_resolution_clock::now();
            double duration_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();

            // Affichage du coup et du temps
            cout << "[" << std::setw(3) << game.currentState.moves_played + 1 << "] "
                 << player_name << " : "
                 << "Field " << std::setw(2) << move.first + 1 << " | Color " << move.second
                 << " (" << std::fixed << std::setprecision(1) << duration_ms << " ms) ";

            // Appliquer le coup
            game.currentState = game_rules.playMove(game.currentState, move.first, move.second);

            // Mettre à jour l'arbre MCTS (Crucial pour qu'il suive le jeu)
            mcts_ai.advance_tree(move, game.currentState);

            cout << "-> " << game.currentState.score_p1 << ":" << game.currentState.score_p2 << endl;

            gameOver = game_rules.gameOver(game.currentState);
        }

        // Fin de la partie
        int score_mcts = mcts_is_p1 ? game.currentState.score_p1 : game.currentState.score_p2;
        int score_minmax = mcts_is_p1 ? game.currentState.score_p2 : game.currentState.score_p1;

        cout << "--------------------------------------------------------" << endl;
        cout << "GAME " << g << " FINISHED." << endl;
        cout << "Score: MCTS " << score_mcts << " - " << score_minmax << " MinMax" << endl;

        if (score_mcts > score_minmax) {
            cout << "WINNER: MCTS" << endl;
            mcts_wins++;
        } else if (score_minmax > score_mcts) {
            cout << "WINNER: MinMax" << endl;
            minmax_wins++;
        } else {
            cout << "RESULT: DRAW" << endl;
            draws++;
        }
        cout << "--------------------------------------------------------" << endl;

        // Sauvegarder l'apprentissage après chaque partie (Sécurité)
        mcts_ai.saveKnowledge("final_games.txt");
    }

    // RÉSULTATS FINAUX DU TOURNOI
    cout << "\n=============================================" << endl;
    cout << "TOURNAMENT FINISHED" << endl;
    cout << "MCTS Wins   : " << mcts_wins << endl;
    cout << "MinMax Wins : " << minmax_wins << endl;
    cout << "Draws       : " << draws << endl;
    cout << "=============================================" << endl;
}

void Game::runStrategyBattle() {
    int num_games = 10;
    int old_wins = 0;
    int new_wins = 0;
    int draws = 0;

    double total_time_new = 0;
    int moves_new = 0;

    // Select Opponent
    int choice;
    std::cout << "Select Opponent for Optimized IDDFS:\n";
    std::cout << "1. Old MinMax (Fixed Depth 6)\n";
    std::cout << "2. Basic IDDFS (Previous Version)\n";
    std::cout << "3. MCTS (Monte Carlo Tree Search)\n";
    std::cin >> choice;

    std::function<std::pair<int, Color>(const State&)> getOpponentMove;
    std::unique_ptr<Bot> oldMinMaxBot;
    std::unique_ptr<BotIDDFS_Old> basicIDDFSBot;
    std::unique_ptr<MCTS> mctsBot;
    std::string opponentName;
    std::string opponentShortName;

    if (choice == 1) {
        oldMinMaxBot = std::make_unique<Bot>(6);
        getOpponentMove = [&](const State& s) { return oldMinMaxBot->getMove(s); };
        opponentName = "Old MinMax (Depth 6)";
        opponentShortName = "OldBot";
    } else if (choice == 2) {
        basicIDDFSBot = std::make_unique<BotIDDFS_Old>(1850);
        getOpponentMove = [&](const State& s) { return basicIDDFSBot->getMove(s); };
        opponentName = "Basic IDDFS";
        opponentShortName = "Basic ";
    } else {
        int iters;
        std::cout << "Enter MCTS Iterations (e.g. 5000): ";
        std::cin >> iters;
        mctsBot = std::make_unique<MCTS>(iters);
        getOpponentMove = [&](const State& s) { return mctsBot->find_best_move(s); };
        opponentName = "MCTS (" + std::to_string(iters) + " iters)";
        opponentShortName = "MCTS  ";
    }

    // New Bot (IDDFS PVS + Killer + History)
    BotIDDFS newBot(1950);

    GameRules game_rules;

    std::cout << "\n=== STRATEGY BATTLE: " << opponentName << " vs IDDFS Optimized ===" << std::endl;

    for (int g = 1; g <= num_games; g++) {
        Game game;
        bool gameOver = false;
        bool newBot_is_p1 = (g % 2 != 0); // Alternate starting player

        if (choice == 3) {
            mctsBot->initialize(game.currentState);
        }

        std::cout << "\nGame " << g << " / " << num_games << " Started!" << std::endl;
        if (newBot_is_p1) std::cout << ">> P1: Optimized vs P2: " << opponentName << std::endl;
        else              std::cout << ">> P1: " << opponentName << " vs P2: Optimized" << std::endl;

        while (!gameOver) {
            std::pair<int, Color> move;
            bool is_p1_turn = game.currentState.player_playing;
            bool is_newBot_turn = (newBot_is_p1 && is_p1_turn) || (!newBot_is_p1 && !is_p1_turn);

            std::string player_name = is_newBot_turn ? "Optim " : opponentShortName;

            auto start_time = std::chrono::high_resolution_clock::now();

            if (is_newBot_turn) {
                move = newBot.getMove(game.currentState);
            } else {
                move = getOpponentMove(game.currentState);
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            double duration_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();

            if (is_newBot_turn) {
                total_time_new += duration_ms;
                moves_new++;
            }

            std::cout << "[" << std::setw(3) << game.currentState.moves_played + 1 << "] "
                      << player_name << " : "
                      << "Field " << std::setw(2) << move.first + 1 << " | Color " << move.second
                      << " (" << std::fixed << std::setprecision(1) << duration_ms << " ms) ";

            game.currentState = game_rules.playMove(game.currentState, move.first, move.second);

            if (choice == 3) {
                mctsBot->advance_tree(move, game.currentState);
            }

            std::cout << "-> " << game.currentState.score_p1 << ":" << game.currentState.score_p2 << std::endl;

            gameOver = game_rules.gameOver(game.currentState);
        }

        int score_new = newBot_is_p1 ? game.currentState.score_p1 : game.currentState.score_p2;
        int score_old = newBot_is_p1 ? game.currentState.score_p2 : game.currentState.score_p1;

        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "Score: Optim " << score_new << " - " << score_old << " " << opponentShortName << std::endl;

        if (score_new > score_old) {
            std::cout << "WINNER: Optimized" << std::endl;
            new_wins++;
        } else if (score_old > score_new) {
            std::cout << "WINNER: " << opponentShortName << std::endl;
            old_wins++;
        } else {
            std::cout << "RESULT: DRAW" << std::endl;
            draws++;
        }
    }

    std::cout << "\n=== BATTLE FINISHED ===" << std::endl;
    std::cout << "Optim Wins : " << new_wins << std::endl;
    std::cout << opponentShortName << " Wins : " << old_wins << std::endl;
    std::cout << "Draws      : " << draws << std::endl;
    if (moves_new > 0) {
        std::cout << "Avg Time IDDFS: " << (total_time_new / moves_new) << " ms" << std::endl;
    }
}

void Game::runCompetition() {
    // Use our Champion Bot
    BotIDDFS bot(1950);
    
    // Ensure output is sent immediately (no buffering)
    std::cout.setf(std::ios::unitbuf);

    std::string token;
    std::atomic<bool> stop_pondering(false);

    while (true) {
        // --- START PONDERING ---
        stop_pondering = false;
        bot.setStopFlag(&stop_pondering);
        bot.setTimeLimit(1000000); // Infinite time for pondering
        
        std::thread ponderThread([&]() {
            bot.getMove(currentState); // Ponder on current state
        });

        if (!(std::cin >> token)) {
            stop_pondering = true;
            ponderThread.join();
            break;
        }

        // --- STOP PONDERING ---
        stop_pondering = true;
        ponderThread.join();
        
        bot.setStopFlag(nullptr);
        bot.setTimeLimit(2850);

        if (token == "START") {

            
            auto move = bot.getMove(currentState);
            
            std::string colorStr;
            switch(move.second) {
                case red: colorStr = "R"; break;
                case blue: colorStr = "B"; break;
                case transparentRED: colorStr = "TR"; break;
                case transparentBLUE: colorStr = "TB"; break;
            }
            std::cout << (move.first + 1) << " " << colorStr << std::endl;
            
            currentState = GameRules::playMove(currentState, move.first, move.second);
            
        } else if (token.find("RESULT") != std::string::npos) {
            break;
        } else {

            int field = std::stoi(token);
            std::string colorStr;
            std::cin >> colorStr;
            
            Color colorVal;
            if (colorStr == "R") colorVal = red;
            else if (colorStr == "B") colorVal = blue;
            else if (colorStr == "TR") colorVal = transparentRED;
            else if (colorStr == "TB") colorVal = transparentBLUE;
            
            // Apply opponent's move
            currentState = GameRules::playMove(currentState, field - 1, colorVal);
            
            if (GameRules::gameOver(currentState)) break;
            
            // Calculate and play our move
            auto move = bot.getMove(currentState);
            std::string myColorStr;
            switch(move.second) {
                case red: myColorStr = "R"; break;
                case blue: myColorStr = "B"; break;
                case transparentRED: myColorStr = "TR"; break;
                case transparentBLUE: myColorStr = "TB"; break;
            }
            std::cout << (move.first + 1) << " " << myColorStr << std::endl;
            currentState = GameRules::playMove(currentState, move.first, move.second);
        }
    }
}