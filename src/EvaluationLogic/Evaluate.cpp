//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/Evaluate.h"

#include "GameRules.h"

// maximizing_player_is_p1 should be true if the bot calling MinMax is P1
int Evaluate::evaluate_state(const State &state, bool maximizing_player_is_p1) {
    // --- LEGACY BASELINE (Pure Score Difference) ---
    constexpr int WIN_SCORE = 10000;

    // 1. GAME OVER CHECK
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

int Evaluate::evaluate_stateNEW(const State &state, bool maximizing_player_is_p1) {
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
    // Weighting: Real points are the priority.
    int real_score_diff = maximizing_player_is_p1
        ? (state.score_p1 - state.score_p2)
        : (state.score_p2 - state.score_p1);

    // We multiply real points so that heuristic potential (positioning)
    // never outweighs a concrete lead in captured seeds.
    // SCALING: 1 Seed = 100 Points.
    double score = real_score_diff * 200.0;

    // TIME PENALTY: -1 point per move. Breaks infinite loops.
    score -= (double)state.moves_played;

    // 3. HEURISTICS (Board Analysis)
    // P1 controls odd indices (0, 2, ...), P2 controls even indices (1, 3, ...)
    // Note: Adjust indices if your logic is 1-based, but usually arrays are 0-based.
    // Based on previous context: P1 (Odd) might mean index 0 if mapped that way.
    // Assuming: maximizing_player_is_p1 -> index 0, 2, 4...
    int player_start_idx = maximizing_player_is_p1 ? 0 : 1;

    int my_mobility = 0;
    int opp_mobility = 0;

    for (int i = 0; i < 16; i++) {
        int seeds = state.board.fields[i].total_seeds;
        bool is_my_hole = (i % 2 == player_start_idx);

        // --- MOBILITY CHECK ---
        // Count how many playable moves each player has.
        if (seeds > 0) {
            if (is_my_hole) my_mobility++;
            else opp_mobility++;
        }

        if (!is_my_hole) {
            // --- OFFENSIVE OPPORTUNITIES (Opponent's side) ---

            // Opponent hole has 1 or 2 seeds -> They are vulnerable to capture.
            // We give a small bonus, but not too large because they might defend it.
            if (seeds == 1 || seeds == 2) {
                score += 30.0; // Increased aggression
            }
            // Opponent hole is empty -> Bad for us, we cannot capture anything there.
            else if (seeds == 0) {
                score -= 5.0;
            }

        } else {
            // --- DEFENSE & BUILD-UP (My side) ---

            // I am vulnerable! Opponent could capture my seeds.
            // We penalize this heavily to teach the bot defense.
            if (seeds == 1 || seeds == 2) {
                score -= 40.0; // Increased defense priority
            }

            // "Battery" Bonus: accumulating many seeds is a strong strategy
            // to perform a "Kroo" (grand slam) later.
            if (seeds > 8) {
                score += 10.0; // Scaled x10
            }
        }
    }

    // 4. STARVATION STRATEGY (Protection & Aggression)
    
    // A) Self-Preservation: Avoid running out of moves.
    if (my_mobility == 0) {
        score -= 50000.0; // Massive penalty (Loss condition) - Increased to outweigh seeds
    } else if (my_mobility == 1) {
        score -= 500.0;   // DANGER: Only 1 move left. High penalty (worth ~2.5 seeds).
    } else if (my_mobility == 2) {
        score -= 100.0;   // CAUTION: Only 2 moves left.
    }

    // B) Aggression: Force opponent into starvation.
    if (opp_mobility == 0) {
        score += 50000.0; // Massive bonus (Win condition)
    } else if (opp_mobility == 1) {
        score += 600.0;   // TRAP: Opponent has only 1 move. Force them here!
    }

    // C) General Mobility Pressure
    // We want to maximize our options and minimize theirs.
    score += (my_mobility * 20.0);
    score -= (opp_mobility * 30.0); // Penalize opponent having options heavily.

    return (int)score;
}