cmake_minimum_required(VERSION 3.3)
project(NCAA_C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp src/Team.cpp src/Team.h src/Pct.cpp src/Pct.h src/Pcts.cpp src/Pcts.h src/Game.cpp src/Game.h src/TeamGame.cpp src/TeamGame.h)
add_executable(NCAA_C ${SOURCE_FILES})