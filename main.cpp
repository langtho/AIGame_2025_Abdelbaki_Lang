// main.cpp
// Minimal C++ program skeleton for an AI game loop (C++17)

#include <iostream>
#include <string>
#include "Game.h"

using namespace std;
int main(int argc, char** argv)
{
   Game game;
   //game.runStrategyBattle();
   //game.runCompetition();
  
   if (argc > 1 && std::string(argv[1]) == "JoueurB") {
       game.runCompetitionNoPondering(false);
   } else {
       game.runCompetitionNoPondering(true);
   }
}