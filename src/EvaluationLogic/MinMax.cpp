//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/MinMax.h"

#include <limits>

#include "GameRules.h"
#include "EvaluationLogic/Evaluate.h"

int MinMax::_minmax(const State &state, int depth, bool maximizing_player, bool original_player_is_p1, int alpha, int beta) {
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
            //state.afficherState(move.first,move.second,depth); //DEBUG
            State next_State = GameRules::playMove(state,move.first,move.second);
            int eval = _minmax(next_State,depth-1,false,original_player_is_p1,alpha,beta);
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
            //state.afficherState(move.first,move.second,depth); //DEBUG
            State next_State = GameRules::playMove(state,move.first,move.second);
            int eval = _minmax(next_State,depth-1,true,original_player_is_p1,alpha,beta);
            min_eval=min(min_eval,eval);

            beta=min(beta,min_eval);
            if (beta<=alpha) {
                break;
            }
        }
        return min_eval;
    }
}

pair<int, Color> MinMax::find_best_move(const State &state) {
    pair<int,Color> best_move={};
    int best_eval = numeric_limits<int>::min();

    int alpha=numeric_limits<int>::min();
    int beta=numeric_limits<int>::max();

    vector<pair<int, Color>> moves = GameRules::getPossibleMoves(state);

    if (moves.empty()) return {};

    // Remember which player we are (the one currently playing)
    bool we_are_p1 = state.player_playing;

    // We are the maximizing player at the root
    for (const auto& move:moves) {
        //state.afficherState(move.first,move.second,max_depth); //DEBUG

        State next_State = GameRules::playMove(state,move.first,move.second);
        // minimizing
        int eval = _minmax(next_State,max_depth-1,false,we_are_p1,alpha,beta);
        if (eval>best_eval) {
            //cout<<eval<<" "<<best_eval<<" "<<move.first<<" "<<move.second<<endl;
            best_eval=eval;
            best_move=move;
        }
        alpha=max(alpha,best_eval);
    }
    return best_move;
}