//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/Evaluate.h"

#include "GameRules.h"

// maximizing_player_is_p1 should be true if the bot calling MinMax is P1
int Evaluate::evaluate_state(const State &state, bool maximizing_player_is_p1) {
    // --- UPDATED BASELINE (Copy of evaluate_stateNEW) ---
    constexpr int WIN_SCORE = 100000; // Scaled up

    // 1. GAME OVER CHECK
    if (GameRules::gameOver(state)) {
        int score = WIN_SCORE - state.moves_played; // Prefer winning fast / losing slow
        if (state.score_p1 > state.score_p2) {
            return maximizing_player_is_p1 ? score : -score;
        } else if (state.score_p2 > state.score_p1) {
            return maximizing_player_is_p1 ? -score : score;
        } else {
            return 0; // Draw
        }
    }

    // 2. BASE SCORE (The most important factor)
    int real_score_diff = maximizing_player_is_p1
        ? (state.score_p1 - state.score_p2)
        : (state.score_p2 - state.score_p1);

    // SCALING: 1 Seed = 100 Points.
    double score = real_score_diff * 200.0;

    // TIME PENALTY: -1 point per move. Breaks infinite loops.
    score -= (double)state.moves_played;

    // 3. HEURISTICS (Board Analysis)
    int player_start_idx = maximizing_player_is_p1 ? 0 : 1;
    int my_mobility = 0;
    int opp_mobility = 0;

    for (int i = 0; i < 16; i++) {
        int seeds = state.board.fields[i].total_seeds;
        bool is_my_hole = (i % 2 == player_start_idx);

        if (seeds > 0) {
            if (is_my_hole) my_mobility++;
            else opp_mobility++;
        }

        if (!is_my_hole) {
            if (seeds == 1 || seeds == 2) score += 30.0;
            else if (seeds == 0) score -= 5.0;
        } else {
            if (seeds == 1 || seeds == 2) score -= 40.0;
            if (seeds > 8) score += 10.0;
        }
    }

    // 4. STARVATION STRATEGY (Protection & Aggression)
    if (my_mobility == 0) score -= 50000.0;
    else if (my_mobility == 1) score -= 500.0;
    else if (my_mobility == 2) score -= 100.0;

    if (opp_mobility == 0) score += 50000.0;
    else if (opp_mobility == 1) score += 600.0;

    // C) General Mobility Pressure
    score += (my_mobility * 20.0);
    score -= (opp_mobility * 30.0);

    return (int)score;
}