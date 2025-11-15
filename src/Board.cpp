//
// Created by loko on 31/10/2025
//
#include "Board.h"

void Board::showBoard() {
    int x=0;
    for (int r=0; r<2; r++) {
            for (int j=0;j<8;j++) {
                if (x>8) {
                    cout<<"|Field:"<<x+1;
                }else {
                  cout<<"| Field:"<<x+1;
                }
                x++;
            }
            cout<<endl;
            x-=8;
            for (int j=0;j<8;j++) {
                fields[x].show_red();
                x++;
            }
            cout<<endl;
            x-=8;
            for (int j=0;j<8;j++) {
                fields[x].show_blue();
                x++;
            }
            cout<<endl;
            x-=8;
            for (int j=0;j<8;j++) {
                fields[x].show_transparent();
                x++;
            }

        x=8;
        cout<<endl<<endl;
    }
}

int Board::getTotalSeeds() {
    int total=0;
    for (int i=0;i<16;i++) {
        total+=fields[i].total_seeds;
    }
    return total;
}