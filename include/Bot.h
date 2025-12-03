//
// Created by loko on 22/11/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_BOT_H
#define AIGAME_2025_ABDELBAKI_LANG_BOT_H

#include "EvaluationLogic/MinMax.h"
#include "State.h"

class Bot {
private:
    MinMax minmax;
public:
    Bot(int depth) : minmax(depth) {};
    std::pair<int, Color> getMove(const State& state);
};

#endif //AIGAME_2025_ABDELBAKI_LANG_BOT_H