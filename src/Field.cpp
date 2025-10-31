//
// Created by loko on 31/10/2025.
//
#include "Field.h"

void Field::show_field() {
    cout<<"R:"<<red_seeds<<endl;
    cout<<"B:"<<blue_seeds<<endl;
    cout<<"T:"<<transparent_seeds<<endl;
}

void Field::show_red() {
    cout<<"|   RED:"<<red_seeds;
}

void Field::show_blue() {
    cout<<"|  BLUE:"<<blue_seeds;
}

void Field::show_transparent() {
    cout<<"|  TRSP:"<<transparent_seeds;
}

int Field::take_seeds(Color color, int& transparentseeds) {
    int seeds=0;
    switch (color) {
        case red:
            seeds=red_seeds;
            red_seeds=0;
            total_seeds-=seeds;
            break;

        case blue:
            seeds=blue_seeds;
            blue_seeds=0;
            total_seeds-=seeds;
            break;

        case transparentRED:
            seeds=red_seeds;
            transparentseeds=transparent_seeds;
            red_seeds=0;
            transparent_seeds=0;
            total_seeds-=seeds;
            total_seeds-=transparentseeds;
            break;

        case transparentBLUE:
            seeds=blue_seeds;
            transparentseeds=transparent_seeds;
            blue_seeds=0;
            transparent_seeds=0;
            total_seeds-=seeds;
            total_seeds-=transparentseeds;
            break;
    }
    return seeds;
}

void Field::put_seed(Color color) {
    switch (color) {
        case red:
            red_seeds++;
            total_seeds++;
            break;

        case blue:
            blue_seeds++;
            total_seeds++;
            break;

        case transparent:
            transparent_seeds++;
            total_seeds++;
            break;
    }
}

int Field::take_all_seeds() {
    int seeds=total_seeds;
    red_seeds=0;
    blue_seeds=0;
    transparent_seeds=0;
    total_seeds=0;
    return seeds;
}
