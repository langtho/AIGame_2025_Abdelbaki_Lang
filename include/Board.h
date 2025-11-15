//
// Created by loko on 21/10/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_BOARD_H
#define AIGAME_2025_ABDELBAKI_LANG_BOARD_H
#include <iostream>
#include "Field.h"

using namespace std;

class Board {
    public:
        Field* fields;

    Board() {
        fields= new Field[16];
    }

    ~Board() {
        delete fields;
    }

    void showBoard();

    int getTotalSeeds();
};

#endif //AIGAME_2025_ABDELBAKI_LANG_BOARD_H