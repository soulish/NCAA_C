cmake_minimum_required(VERSION 3.3)
project(NCAA_C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu/)
find_package(Boost COMPONENTS date_time REQUIRED)
link_directories(${Boost_LIBRARY_DIR})

set(SOURCE_FILES main.cpp src/Team.cpp src/Team.h src/Pct.cpp src/Pct.h src/Pcts.cpp src/Pcts.h src/Game.cpp src/Game.h src/TeamGame.cpp src/TeamGame.h src/TeamAverage.cpp src/TeamAverage.h src/TeamWAverage.cpp src/TeamWAverage.h src/readTeams.h helpers/doubleFormatter.h)
add_executable(NCAA_C ${SOURCE_FILES})
target_link_libraries( NCAA_C ${Boost_LIBRARIES} )

set(SOURCE_FILES_2 gen_averages.cpp src/Team.cpp src/Team.h src/Pct.cpp src/Pct.h src/Pcts.cpp src/Pcts.h src/Game.cpp src/Game.h src/TeamGame.cpp src/TeamGame.h src/TeamAverage.cpp src/TeamAverage.h src/TeamWAverage.cpp src/TeamWAverage.h src/readTeams.h helpers/doubleFormatter.h)
add_executable(gen_averages ${SOURCE_FILES_2})
target_link_libraries( gen_averages ${Boost_LIBRARIES} )
