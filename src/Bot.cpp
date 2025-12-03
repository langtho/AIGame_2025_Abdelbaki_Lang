//
// Created by loko on 22/11/2025.
//

#include "Bot.h"

std::pair<int, Color> Bot::getMove(const State& state) {
    return minmax.find_best_move(state);
}