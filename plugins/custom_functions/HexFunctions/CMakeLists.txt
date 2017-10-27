cmake_minimum_required(VERSION 2.6)
project(HexFunctions)

set(HexFunctions_src src/HexFunctions.cpp src/HexUtil.cpp)
include_directories(${DmxCustomFunctions_SOURCE_DIR}/include)
include_directories(${HexFunctions_SOURCE_DIR}/include)

add_library(HexFunctions SHARED ${HexFunctions_src})

