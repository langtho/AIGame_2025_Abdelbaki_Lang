//
// Created by loko on 31/10/2025.
//

#include "Game.h"

int Game::playMove(int field, Color color) {
    bool distributing_in_all_holes = false;
    int seeds=0;
    int transparent_seeds=0;
    int score=0;

    if (color==red || color==transparentRED)distributing_in_all_holes=true;

    seeds=board->fields[field].take_seeds(color,transparent_seeds);

    if (seeds==0) {
        cerr<<"Your play isn't valid: No seed in the demanded color"<<endl;
    }

    field++;

    while (seeds!=0 ||transparent_seeds!=0) {

        if (transparent_seeds!=0) {
            board->fields[field].put_seed(transparent);
            transparent_seeds--;
        }else {
            (distributing_in_all_holes)?board->fields[field].put_seed(red):board->fields[field].put_seed(blue);
            seeds--;
        }

        if (seeds==0) {
            score=capture(field,distributing_in_all_holes);
            break;
        }

        if (distributing_in_all_holes) {
            if (field!=15) {
                field++;
            }else {
                field=0;
            }
        }else {
            if (field==14) {
                field=0;
            }else if (field==15) {
                field=1;
            }else {
                field+=2;
            }
        }
    }
    return score;
}

int Game::capture(int field, bool everyField) {
    bool capture=true;
    int score=0;
    while (capture) {
        if (board->fields[field].total_seeds==2 || board->fields[field].total_seeds==3) {
            score+=board->fields[field].take_all_seeds();
            if (everyField) {
                if (field==0) {
                    field=15;
                }else {
                 field--;
                }

            }else {
                if (field==0) {
                    field=14;
                }else if (field==1) {
                    field=15;
                }else {
                    field-=2;
                }
            }
        }else {
            return score;
        }
    }
}


bool Game::gameOver() {
    if (score_p1 > 49 || score_p2>49 || score_p1+score_p2>70) {
        return true;
    }
    return false;
}

Board* Game::getBoard() {
    return board;
};
int Game::getScoreP1() {
    return score_p1;
}
int Game::getScoreP2() {
    return score_p2;
}

void Game::setScoreP1(int score) {
    score_p1 += score;
}
void Game::setScoreP2(int score) {
    score_p2 += score;
}