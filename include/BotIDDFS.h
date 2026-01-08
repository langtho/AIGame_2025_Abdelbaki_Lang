#ifndef AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_H
#define AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_H

#include "EvaluationLogic/MinMaxIDDFS.h"
#include "State.h"

class BotIDDFS {
private:
    MinMaxIDDFS ai;
public:
    // time_ms: Time limit per move in milliseconds
    BotIDDFS(int time_ms = 1950) : ai(time_ms) {};
    std::pair<int, Color> getMove(const State& state);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_H