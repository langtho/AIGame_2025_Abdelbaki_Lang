//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/Evaluate.h"

#include "GameRules.h"

int Evaluate::evaluate_state(const State &state) {
    if (GameRules::gameOver(state)) {
        constexpr int WIN_SCORE = 1000;
        if (state.score_p1>state.score_p2) {
            return WIN_SCORE;
        }else {
            return -WIN_SCORE;
        }
    }
    return state.score_p1-state.score_p2;
}
