//
// Created by loko on 21/10/2025.
//

#ifndef AIGAME_2025_ABDELBAKI_LANG_FIELD_H
#define AIGAME_2025_ABDELBAKI_LANG_FIELD_H
#include <iostream>
#include "Color.h"
using namespace std;

class Field {
    public:
        int red_seeds, blue_seeds , transparent_seeds;
        int total_seeds;

    Field():red_seeds(2),blue_seeds(2),transparent_seeds(2),total_seeds(6){}

    void show_field();
    void show_red();
    void show_blue();
    void show_transparent();

    int take_seeds(Color color,int& transparentSeeds);
    int take_all_seeds();
    void put_seed(Color color);
};


#endif //AIGAME_2025_ABDELBAKI_LANG_FIELD_H