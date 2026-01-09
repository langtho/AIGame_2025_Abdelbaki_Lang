//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/MinMax.h"


#include <limits>
#include <chrono>

#include "GameRules.h"
#include "EvaluationLogic/Evaluate.h"

int MinMax::_minmax(const State &state, int depth, bool maximizing_player, bool original_player_is_p1, int alpha, int beta, const std::chrono::steady_clock::time_point& start_time, std::chrono::milliseconds timeout) {
// Check for timeout
auto now = std::chrono::steady_clock::now();
if (now - start_time > timeout) {
    timeout_reached = true;
    return 0;
}

    if (depth==0|| GameRules::gameOver(state)) {
        return Evaluate::evaluate_state(state, original_player_is_p1);
    }

    auto moves = GameRules::getPossibleMoves(state);

    if (moves.empty()) {
        return Evaluate::evaluate_state(state, original_player_is_p1);
    }

    if (maximizing_player) {
        int max_eval = numeric_limits<int>::min();
        for (const auto& move : moves) {
            State next_State = GameRules::playMove(state,move.first,move.second);
            int eval = _minmax(next_State,depth-1,false,original_player_is_p1,alpha,beta,start_time,timeout);
            max_eval=max(max_eval,eval);

            alpha=max(alpha,max_eval);
            if (beta<=alpha) {
                break;
            }
        }
        return max_eval;
    }else {
        int min_eval = numeric_limits<int>::max();
        for (const auto& move:moves ) {
            State next_State = GameRules::playMove(state,move.first,move.second);
            int eval = _minmax(next_State,depth-1,true,original_player_is_p1,alpha,beta,start_time,timeout);
            min_eval=min(min_eval,eval);

            beta=min(beta,min_eval);
            if (beta<=alpha) {
                break;
            }
        }
        return min_eval;
    }
}

pair<int, Color> MinMax::find_best_move(const State &state, std::chrono::milliseconds timeout) {
    std::vector<std::pair<int, Color>> moves = GameRules::getPossibleMoves(state);

    if (moves.empty()) return {};

    // Remember which player we are (the one currently playing)
    bool we_are_p1 = state.player_playing;

    // Start timing
    auto start_time = std::chrono::steady_clock::now();

    // Best move from the last fully completed depth (iterative deepening)
    pair<int,Color> best_move = moves[0]; // Default to first move
    int best_eval_completed = std::numeric_limits<int>::min();

    // Iterative deepening: search from depth 1 to max_depth
    for (int current_depth = 1; current_depth <= max_depth; current_depth++) {
        timeout_reached = false;
        
        pair<int,Color> best_move_this_depth = {};
        int best_eval_this_depth = std::numeric_limits<int>::min();
        int alpha = std::numeric_limits<int>::min();
        int beta = std::numeric_limits<int>::max();

        // Search all moves at current depth
        for (const auto& move : moves) {
            // Check for timeout before each move
            auto now = std::chrono::steady_clock::now();
            if (now - start_time > timeout) {
                timeout_reached = true;
                break;
            }

            State next_State = GameRules::playMove(state, move.first, move.second);
            int eval = _minmax(next_State, current_depth - 1, false, we_are_p1, alpha, beta, start_time, timeout);
            
            if (timeout_reached) {
                break;
            }

            if (eval > best_eval_this_depth) {
                best_eval_this_depth = eval;
                best_move_this_depth = move;
            }
            alpha = std::max(alpha, best_eval_this_depth);
        }

        // Only update best move if this depth completed fully
        if (!timeout_reached) {
            best_move = best_move_this_depth;
            best_eval_completed = best_eval_this_depth;
            //std::cout << "Depth " << current_depth << " completed, best eval: " << best_eval_completed << std::endl;
        } else {
            //std::cout << "Timeout at depth " << current_depth << ", using depth " << (current_depth - 1) << std::endl;
            break;
        }
    }

    return best_move;
}