#include "BotIDDFS.h"

// Delegates the move calculation to the Engine.
std::pair<int, Color> BotIDDFS::getMove(const State& state) {
    return ai.find_best_move(state);
}