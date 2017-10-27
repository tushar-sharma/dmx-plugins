cmake_minimum_required(VERSION 3.6)
project(HexFunctions)

set(HexFunctions_src HexFunctions.cpp )
include_directories(${DmxCustomFunctions_SOURCE_DIR}/include)
#include_directories(${HexFunctions_SOURCE_DIR})

add_library(HexFunctions SHARED ${HexFunctions_src})


