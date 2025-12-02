//
// Created by loko on 21/10/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_BOARD_H
#define AIGAME_2025_ABDELBAKI_LANG_BOARD_H
#include <iostream>
#include "Field.h"
#include <vector>
using namespace std;

class Board {
    public:
        vector<Field> fields;

    Board() {
        fields= vector<Field>(16);
    }

    ~Board() {

    }
    Board(const Board& other) =default;
    void showBoard() const;
};

#endif //AIGAME_2025_ABDELBAKI_LANG_BOARD_H