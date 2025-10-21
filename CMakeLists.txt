cmake_minimum_required(VERSION 4.0)
project(AIGame_2025_Abdelbaki_Lang)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(AIGame_2025_Abdelbaki_Lang
        include/Color.h
        include/Seed.h
        main.cpp)
