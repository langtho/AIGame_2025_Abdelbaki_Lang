//
// Created by loko on 31/10/2025.
//

#include "Game.h"
#include "GameRules.h"
#include "BotIDDFS.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>


void Game::runCompetition() {
    // Initialize our Bot with a time limit of 1900ms (safe buffer for 2s limit).
    BotIDDFS bot(1900);
    std::cout.setf(std::ios::unitbuf);

    std::string token;
    std::atomic<bool> stop_pondering(false);

    while (true) {
        // --- START PONDERING ---
        // While we wait for the opponent to play, we use the CPU to think ahead.
        // We run the search on the current state (where it is the opponent's turn).
        // This fills the Transposition Table with evaluations of the opponent's possible moves.
        stop_pondering = false;
        bot.setStopFlag(&stop_pondering);
        bot.setTimeLimit(1000000); // Infinite time for pondering
        
        std::thread ponderThread([&]() {
            bot.getMove(currentState); // Start search in background
        });

        // Block and wait for input from the arbiter
        if (!(std::cin >> token)) {
            stop_pondering = true;
            ponderThread.join();
            break;
        }

        // --- STOP PONDERING ---
        // Input received! Stop the background search immediately.
        stop_pondering = true;
        ponderThread.join();
        
        // Reset bot settings for the actual search
        bot.setStopFlag(nullptr);
        bot.setTimeLimit(2850);

        if (token == "START") {
            // We are Player 1 and we start.
            
            auto move = bot.getMove(currentState);
            
            // Convert move to string format (R, B, TR, TB)
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
            // Game Over signal
            break;
        } else {
            // Opponent played.
            // Parse the move: Field (token) and Color.
            int field = std::stoi(token);
            std::string colorStr;
            std::cin >> colorStr;
            
            Color colorVal;
            if (colorStr == "R") colorVal = red;
            else if (colorStr == "B") colorVal = blue;
            else if (colorStr == "TR") colorVal = transparentRED;
            else if (colorStr == "TB") colorVal = transparentBLUE;
            
            // Apply opponent's move to our internal state
            currentState = GameRules::playMove(currentState, field - 1, colorVal);
            
            if (GameRules::gameOver(currentState)) break;
            
            // Calculate our best move
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
        if (GameRules::gameOver(currentState)) {
            std::string winner="Draw";
            if (currentState.score_p1 > currentState.score_p2) winner = "1";
            else if (currentState.score_p2 > currentState.score_p1) winner = "2";
            std::cerr << "RESULT " << winner << currentState.score_p1<<":"<<currentState.score_p2<<std::endl;
            break;
        }
    }
}