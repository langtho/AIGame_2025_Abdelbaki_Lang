//
// Created by loko on 22/11/2025.
//
#include "GameRules.h"

State GameRules::playMove(const State& state,int field, Color color) {
    bool distributing_in_all_holes = false;
    int seeds=0;
    int transparent_seeds=0;
    int score=0;
    State newState=State(state);
    if (color==red || color==transparentRED)distributing_in_all_holes=true;

    seeds=newState.board.fields[field].take_seeds(color,transparent_seeds);

    if (seeds==0) {
        cerr<<"Your play isn't valid: No seed in the demanded color"<<endl;
    }

    field++;

    while (seeds!=0 ||transparent_seeds!=0) {

        if (transparent_seeds!=0) {
            newState.board.fields[field].put_seed(transparent);
            transparent_seeds--;
        }else {
            (distributing_in_all_holes)?newState.board.fields[field].put_seed(red):newState.board.fields[field].put_seed(blue);
            seeds--;
        }

        if (seeds==0) {

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
    score=capture(newState,field);
    if (newState.player_playing) {
        newState.score_p1+=score;
    }else {
        newState.score_p2+=score;
    }
    newState.player_playing=!newState.player_playing;
    return newState;
}

bool GameRules::gameOver(const State& state) {
    if (state.score_p1 > 49 || state.score_p2>49 || state.score_p1+state.score_p2>70) {
        return true;
    }
    return false;
}

int GameRules::capture(State& state,int field) {
    bool capture=true;
    int score=0;
    while (capture) {
        if (state.board.fields[field].total_seeds==2 || state.board.fields[field].total_seeds==3) {
            score+=state.board.fields[field].take_all_seeds();

                if (field==0) {
                    field=15;
                }else {
                    field--;
                }
        }else {
            return score;
        }
    }
}
