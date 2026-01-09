// main.cpp
// Minimal C++ program skeleton for an AI game loop (C++17)

#include <iostream>
#include <cstring>
#include "Game.h"

using namespace std;
int main(int argc, char** argv)
{
   Game game;
   if (argc > 1 && (strcmp(argv[1], "JoueurA") == 0 || strcmp(argv[1], "JoueurB") == 0)) {
       game.runAsExternalPlayer();
   } else {
       game.runAIBattle();
   }
   return 0;
}