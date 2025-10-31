// main.cpp
// Minimal C++ program skeleton for an AI game loop (C++17)

#include <iostream>
#include "Game.h"

using namespace std;
int main(int argc, char** argv)
{
   bool gameOver=false;
   bool player1turn=true;
   Game game;
   while (!gameOver) {
      cout<<"Score P1: "<<game.getScoreP1()<<"Score P2: "<<game.getScoreP2()<<endl;
      game.getBoard()->showBoard();
      if (player1turn) {
         cout<<"Player1 turn"<<endl;
      }else {
         cout<<"Player2 turn"<<endl;
      }
      int field;
      int color;
      cout<<"Choose your Field: "<<endl;
      cin>>field;
      cout<<"Choose your Color: "<<endl<<"0:red"<<endl<<"1:blue"<<endl<<"2:transparent red"<<endl<<"3:transparent blue"<<endl;
      cin>>color;
      int points=game.playMove(field,(Color)color);

      if (points>0) {
         (player1turn)?game.setScoreP1(points):game.setScoreP2(points);
      }
      gameOver=game.gameOver();
      player1turn=!player1turn;

      system("cls");
   }

   cout<<"Game is over P1: "<<game.getScoreP1()<<"Score P2: "<<game.getScoreP2()<<endl;
}