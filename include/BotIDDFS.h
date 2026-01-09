#ifndef AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_H
#define AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_H

#include "EvaluationLogic/MinMaxIDDFS.h"
#include "State.h"
#include <atomic>

// Wrapper class for the MinMaxIDDFS engine.
class BotIDDFS {
private:
    MinMaxIDDFS ai;
public:
    // Constructor: Sets the time limit per move (in milliseconds).
    BotIDDFS(int time_ms = 1950) : ai(time_ms) {};
    std::pair<int, Color> getMove(const State& state);
    void setTimeLimit(int ms) { ai.setTimeLimit(ms); }
    void setStopFlag(std::atomic<bool>* flag) { ai.setStopFlag(flag); }
};

#endif //AIGAME_2025_ABDELBAKI_LANG_BOTIDDFS_H