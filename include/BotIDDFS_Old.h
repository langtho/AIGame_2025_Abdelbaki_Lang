#ifndef AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_OLD_H
#define AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_OLD_H

#include "EvaluationLogic/MinMaxIDDFS_Old.h"
#include "State.h"

class BotIDDFS_Old {
private:
    MinMaxIDDFS_Old ai;
public:
    BotIDDFS_Old(int time_ms = 1850) : ai(time_ms) {};
    std::pair<int, Color> getMove(const State& state) { return ai.find_best_move(state); }
};

#endif