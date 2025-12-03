//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/Evaluate.h"

#include "GameRules.h"

// maximizing_player_is_p1 should be true if the bot calling MinMax is P1
int Evaluate::evaluate_state(const State &state, bool maximizing_player_is_p1) {
    constexpr int WIN_SCORE = 1000;
    
    if (GameRules::gameOver(state)) {
        if (state.score_p1 > state.score_p2) {
            return maximizing_player_is_p1 ? WIN_SCORE : -WIN_SCORE;
        } else if (state.score_p2 > state.score_p1) {
            return maximizing_player_is_p1 ? -WIN_SCORE : WIN_SCORE;
        } else {
            return 0; // Draw
        }
    }
    
    // Return score difference from maximizing player's perspective
    return maximizing_player_is_p1 
        ? (state.score_p1 - state.score_p2) 
        : (state.score_p2 - state.score_p1);
}
