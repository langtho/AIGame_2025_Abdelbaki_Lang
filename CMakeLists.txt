cmake_minimum_required(VERSION 3.20)
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
        include/GameRules.h
        src/GameRules.cpp
        src/EvaluationLogic/Evaluate.cpp
        src/BotIDDFS.cpp
        src/EvaluationLogic/MinMaxIDDFS.cpp

)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # RELEASE MODE: -O3 speed, -DNDEBUG to disable asserts, -march=native for CPU features, -pthread for threading
    target_compile_options(${PROJECT_NAME} PRIVATE -march=native -pthread)
    target_link_options(${PROJECT_NAME} PRIVATE -pthread)
endif()
