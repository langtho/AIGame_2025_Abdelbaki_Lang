#include "BotIDDFS.h"

std::pair<int, Color> BotIDDFS::getMove(const State& state) {
    return ai.find_best_move(state);
}