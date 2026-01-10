//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/Evaluate.h"

#include "GameRules.h"

// Evaluates the board state from the perspective of the maximizing player.
// Returns a score: positive if maximizing player is winning, negative if losing.
int Evaluate::evaluate_state(const State &state, bool maximizing_player_is_p1) {
    // A very high score representing a guaranteed win.
    constexpr int WIN_SCORE = 100000;

    // 1. GAME OVER CHECK:
    // If the game is over, return a massive score.
    // We subtract moves_played to prefer winning faster (or losing slower).
    if (GameRules::gameOver(state)) {
        int score = WIN_SCORE - state.moves_played;
        if (state.score_p1 > state.score_p2) {
            return maximizing_player_is_p1 ? score : -score;
        } else if (state.score_p2 > state.score_p1) {
            return maximizing_player_is_p1 ? -score : score;
        } else {
            return 0; // Draw
        }
    }

    // 2. BASE SCORE:
    // The actual score difference is the most important factor.
    int real_score_diff = maximizing_player_is_p1
        ? (state.score_p1 - state.score_p2)
        : (state.score_p2 - state.score_p1);

    // SCALING:
    // We multiply by 200 so that 1 seed = 200 points.
    // This allows us to add smaller heuristic bonuses/penalties without overriding the seed count.
    double score = real_score_diff * 200.0;

    // TIME PENALTY:
    // Subtract 1 point per move to encourage progress and break infinite loops.
    score -= (double)state.moves_played;

    // 3. HEURISTICS (Board Analysis):
    // We analyze the board to reward good positioning and punish vulnerability.
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
            // Opponent's side:
            // If they have 1 or 2 seeds, they are vulnerable to capture. Good for us.
            if (seeds == 1 || seeds == 2) score += 30.0;
            // If empty, we can't capture there. Slight penalty.
            else if (seeds == 0) score -= 5.0;
        } else {
            // My side:
            // If I have 1 or 2 seeds, I am vulnerable. Bad for us.
            if (seeds == 1 || seeds == 2) score -= 40.0;
            // Hoarding seeds (battery) can be a strong strategy for a big move later.
            if (seeds > 8) score += 10.0;
        }
    }

    // 4. STARVATION STRATEGY:
    // A) Self-Preservation: Avoid running out of moves (Starvation = Loss).
    if (my_mobility == 0) score -= 50000.0; // Massive penalty, almost as bad as losing.
    else if (my_mobility == 1) score -= 500.0; // Danger zone.
    else if (my_mobility == 2) score -= 100.0; // Caution.

    // B) Aggression: Try to starve the opponent.
    if (opp_mobility == 0) score += 50000.0; // Massive bonus, almost a win.
    else if (opp_mobility == 1) score += 600.0; // Trap them!

    // C) General Mobility Pressure:
    // Reward having more options, punish opponent having options.
    score += (my_mobility * 20.0);
    score -= (opp_mobility * 30.0);

    return (int)score;
}