cmake_minimum_required(VERSION 4.0)
project(AIGame_2025_Abdelbaki_Lang)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(AIGame_2025_Abdelbaki_Lang
        main.cpp
        include/Field.h
        include/Board.h
        src/Board.cpp
        src/Field.cpp
        include/Game.h
        src/Game.cpp
        include/Color.h
        include/State.h
        include/EvaluationLogic/Evaluate.h
        include/EvaluationLogic/MinMax.h
        include/GameRules.h
        src/GameRules.cpp)
