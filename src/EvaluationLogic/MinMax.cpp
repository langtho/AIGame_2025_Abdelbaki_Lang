//
// Created by loko on 22/11/2025.
//
#include "EvaluationLogic/MinMax.h"

#include <limits>

#include "GameRules.h"
#include "EvaluationLogic/Evaluate.h"

int MinMax::_minmax(const State &state, int depth, bool player_playing,int alpha,int beta) {
    if (depth==0|| GameRules::gameOver(state)) {
        return Evaluate::evaluate_state(state);
    }

    auto moves = GameRules::getPossibleMoves(state);

    if (player_playing) {
        int max_eval = numeric_limits<int>::min();
        for (auto move:moves ) {
            //state.afficherState(move.first,move.second,depth); //DEBUG
            State next_State = GameRules::playMove(state,move.first,move.second);
            int eval = _minmax(next_State,depth-1,false,alpha,beta);
            max_eval=max(max_eval,eval);

            alpha=max(alpha,max_eval);
            if (beta<=alpha) {
                break;
            }
        }
        return max_eval;
    }else {
        int min_eval = numeric_limits<int>::max();
        for (auto move:moves ) {
            //state.afficherState(move.first,move.second,depth); //DEBUG
            State next_State = GameRules::playMove(state,move.first,move.second);
            int eval = _minmax(next_State,depth-1,true,alpha,beta);
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
    int max_eval = numeric_limits<int>::min();

    int alpha=numeric_limits<int>::min();
    int beta=numeric_limits<int>::max();
    vector<pair<int,Color>> moves ;

   moves = GameRules::getPossibleMoves(state);

    if (moves.empty()) return {};

    for (auto move:moves) {
        //state.afficherState(move.first,move.second,max_depth); //DEBUG

        State next_State = GameRules::playMove(state,move.first,move.second);
        int eval = _minmax(next_State,max_depth-1,false,alpha,beta);
        if (eval>max_eval) {
            //cout<<eval<<" "<<max_eval<<" "<<move.first<<" "<<move.second<<endl;
            max_eval=eval;
            best_move=move;
        }
        alpha=max(alpha,max_eval);
    }
    return best_move;
}