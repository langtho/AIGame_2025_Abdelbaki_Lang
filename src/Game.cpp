//
// Created by loko on 31/10/2025.
// updated by Kacem le 04/11/2025 (updated the gameOver method logic)
//

#include "Game.h"
#include <vector>

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

    std::vector<int> sown;

    while (seeds!=0 ||transparent_seeds!=0) {
        if (transparent_seeds!=0) {
            board->fields[field].put_seed(transparent);
            transparent_seeds--;
        }else {
            (distributing_in_all_holes)?board->fields[field].put_seed(red):board->fields[field].put_seed(blue);
            seeds--;
        }

        sown.push_back(field);

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
    for (int f : sown) {
        if (board->fields[f].total_seeds == 2 || board->fields[f].total_seeds == 3) {
            score += capture(f, distributing_in_all_holes);
        }
    }
    return score;
}


/*
Capturing:
  * occurs only when a player brings the count of an hole to exactly two or three seeds (of any color). This always captures the seeds in the corresponding hole, and possibly more: If the previous-to-last seed also brought an hole to two or three seeeds, these are captured as well, and so on until a hole is reached which does not contain two or three seeds. The captured seeds are set aside. Starving the opponent IS ALLOWED
  * it is allowed to take the seeds from its own hole and seeds are captured (independently of their colors).
  * Taking all the seeds of the opponent is allowed (all the seeds are captured by the last player).
  * If there's less than 10 seeds on the board. In this case, the remaining seeds are not take into account.
*/
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

/*
The game is over when: 
    * one player has captured 49 or more seeds
    * each player has taken 40 seeds (draw)
    * there is only strictly less than 10 seeds that remain

==> The winner is the player who has more seeds than his opponent
*/
bool Game::gameOver() {
	int remainingSeeds = board->getTotalSeeds();

    if (score_p1 > 49 || score_p2>49 || remainingSeeds<=10 || (score_p1 == 40 && score_p2 == 40)) {
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