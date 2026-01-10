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

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if(ENABLE_ASAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${PROJECT_NAME} PRIVATE 
            -fsanitize=address 
            -g 
            -O1 
            -fno-omit-frame-pointer
        )        
        target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
        message(STATUS "AddressSanitizer enabled for ${PROJECT_NAME}")
    else()
        message(WARNING "AddressSanitizer not supported for this compiler")
    endif()
endif()